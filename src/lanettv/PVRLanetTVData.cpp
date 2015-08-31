/*
*      Copyright (C) 2014-2015 Anton Baranov
*      http://cryol.kiev.ua/
*
*      Copyright (C) 2011 Pulse-Eight
*      http://www.pulse-eight.com/
*
*  This Program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2, or (at your option)
*  any later version.
*
*  This Program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with Kodi; see the file COPYING.  If not, write to
*  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*  http://www.gnu.org/copyleft/gpl.html
*
*/

#include "PVRLanetTVData.h"
#include <json/json.h>
#include <sstream>

using namespace std;
using namespace ADDON;

PVRLanetTVData::PVRLanetTVData(void)
{
	m_iEpgStart = -1;
	m_strDefaultIcon = std::string(GIT_PLUGIN_URL) + "/theme/images/icon.png";
	m_strDefaultMovie = "";

	XBMC->Log(LOG_DEBUG, "%s - Start load data", __FUNCTION__);

	if (LoadChannelsData())
	{
		XBMC->QueueNotification(QUEUE_INFO, "%d channels loaded.", m_channels.size());
	}
}

PVRLanetTVData::~PVRLanetTVData(void)
{
	m_channels.clear();
}

std::string PVRLanetTVData::LoadFile(std::string url)
{
	XBMC->Log(LOG_DEBUG, "Load File: %s", url.c_str());
	std::string retData;

	void* fileHandle = XBMC->OpenFile(url.c_str(), 0);

	if (fileHandle) {
		XBMC->Log(LOG_DEBUG, "Load Complete");
		char buffer[1024];
		while (int bytesRead = XBMC->ReadFile(fileHandle, buffer, 1024))
			retData.append(buffer, bytesRead);
		XBMC->CloseFile(fileHandle);
	}
	return retData;
}

bool PVRLanetTVData::LoadChannelsData(void)
{
	std::string channels_list = LoadFile(g_strListPath);

	Json::Value channels;
	Json::Reader channels_reader;

	bool parsedSuccess = channels_reader.parse(channels_list, channels, false);
	if (!parsedSuccess) {
		XBMC->Log(LOG_ERROR, "Unable to load playlist file '%s':", channels_reader.getFormattedErrorMessages().c_str());
		return false;
	}

	/* Identify Content Pack */
	m_strListPack = channels["pack"].asString();

	/* load channels */
	int channelList_size = channels["list"].size();

	int iUniqueChannelId = 0;
	for (int i = 0; i < channelList_size; ++i)
	{
		CStdString				strTmp;
		PVRLanetTVChannel		channel;

		channel.iUniqueId = ++iUniqueChannelId;
		channel.iEncryptionSystem = 0;

		Json::Value channelData = channels["list"][i];

		/* channel name */
		if (channelData.isMember("title"))
		{
			strTmp = channelData["title"].asString().c_str();
			XBMC->Log(LOG_DEBUG, "Load channel %s", XBMC->UnknownToUTF8(strTmp));
			channel.strChannelName = XBMC->UnknownToUTF8(strTmp);
		}
		else
		{
			continue;
		}

		/* channel number */
		channel.iChannelNumber = atoi(channelData.get("id", "0").asString().c_str());

		/* channel logo */
		channel.strIconPath = g_strLogoPath + channelData.get("id", "0").asString() + ".png";

		/* stream url */
		channel.strStreamURL = channels["edge"].asString() + channelData.get("id", "0").asString() + ".m3u8";

		m_channels.push_back(channel);
	}

	return true;
}

int PVRLanetTVData::GetChannelsAmount(void)
{
  return m_channels.size();
}

PVR_ERROR PVRLanetTVData::GetChannels(ADDON_HANDLE handle, bool bRadio)
{
	for (unsigned int iChannelPtr = 0; iChannelPtr < m_channels.size(); iChannelPtr++)
	{
		PVRLanetTVChannel &channel = m_channels.at(iChannelPtr);

		PVR_CHANNEL xbmcChannel;
		memset(&xbmcChannel, 0, sizeof(PVR_CHANNEL));

		xbmcChannel.iUniqueId					= channel.iUniqueId;
		xbmcChannel.bIsRadio					= false;
		xbmcChannel.iChannelNumber				= channel.iChannelNumber;
		xbmcChannel.iSubChannelNumber			= channel.iChannelNumber;
		xbmcChannel.iEncryptionSystem			= channel.iEncryptionSystem;
		xbmcChannel.bIsHidden					= false;

		strncpy(xbmcChannel.strChannelName, channel.strChannelName.c_str(), sizeof(xbmcChannel.strChannelName) - 1);
		strncpy(xbmcChannel.strStreamURL, channel.strStreamURL.c_str(), sizeof(xbmcChannel.strStreamURL) - 1);
		strncpy(xbmcChannel.strIconPath, channel.strIconPath.c_str(), sizeof(xbmcChannel.strIconPath) - 1);

		PVR->TransferChannelEntry(handle, &xbmcChannel);
}

	return PVR_ERROR_NO_ERROR;
}

bool PVRLanetTVData::GetChannel(const PVR_CHANNEL &channel, PVRLanetTVChannel &myChannel)
{
  for (unsigned int iChannelPtr = 0; iChannelPtr < m_channels.size(); iChannelPtr++)
  {
    PVRLanetTVChannel &thisChannel = m_channels.at(iChannelPtr);
    if (thisChannel.iUniqueId == (int) channel.iUniqueId)
    {
      myChannel.iUniqueId         = thisChannel.iUniqueId;
      myChannel.iChannelNumber    = thisChannel.iChannelNumber;
      myChannel.iSubChannelNumber = thisChannel.iSubChannelNumber;
      myChannel.iEncryptionSystem = thisChannel.iEncryptionSystem;
      myChannel.strChannelName    = thisChannel.strChannelName;
      myChannel.strIconPath       = thisChannel.strIconPath;
      myChannel.strStreamURL      = thisChannel.strStreamURL;

      return true;
    }
  }

  return false;
}


PVR_ERROR PVRLanetTVData::GetEPGForChannel(ADDON_HANDLE handle, const PVR_CHANNEL &channel, time_t iStart, time_t iEnd)
{
	if (m_iEpgStart == -1)
		m_iEpgStart = iStart;

	time_t iLastEndTime = m_iEpgStart + 1;
	int iAddBroadcastId = 0;
	XBMC->Log(LOG_DEBUG, "GetEPGForChannel start '%d' end '%d'", iStart, iEnd);

	for (unsigned int iChannelPtr = 0; iChannelPtr < m_channels.size(); iChannelPtr++)
	{
		PVRLanetTVChannel &myChannel = m_channels.at(iChannelPtr);
		if (myChannel.iUniqueId != (int) channel.iUniqueId)
			continue;

		XBMC->Log(LOG_DEBUG, "Try load EPG for channel: %s", myChannel.strChannelName.c_str());

		time_t iLastEndTimeTmp = 0;

		std::string strChannelEPG = LoadFile(g_strEPGPath + std::to_string(myChannel.iChannelNumber).c_str() + "/kodi");

		Json::Value epg;
		Json::Reader epg_reader;

		bool parsedSuccess = epg_reader.parse(strChannelEPG, epg, false);
		if (!parsedSuccess) {
			XBMC->Log(LOG_ERROR, "Unable to load epg file '%s':", epg_reader.getFormattedErrorMessages().c_str());
			continue;
		}
		int iBroadCastId = 0;

		for (unsigned int iEntryPtr = 0; iEntryPtr < epg.size(); iEntryPtr++)
		{

			PVRLanetTVEpgEntry entry;
			Json::Value epgData = epg[iEntryPtr];

			/* broadcast id */
			entry.iBroadcastId = ++iBroadCastId;

			/* channel id */
			entry.iChannelId = myChannel.iUniqueId;

			/* title */
			entry.strTitle = XBMC->UnknownToUTF8(epgData.get("title", "None").asString().c_str());

			/* start */
			entry.startTime = atoi(epgData.get("start", "0").asString().c_str());

			/* end */
			entry.endTime = atoi(epgData.get("stop", "0").asString().c_str());

			/* plot(description) */
			entry.strPlot = XBMC->UnknownToUTF8(epgData.get("text", "None").asString().c_str());

			/* genre */
			std::string genre = epgData.get("cat", "0:0").asString().c_str();
			entry.iGenreType = atoi(genre.substr(0, genre.find(":")).c_str())*16;
			entry.iGenreSubType = atoi(genre.substr(genre.find(":"), genre.length()).c_str());

			/* entry.strGenreDescription = XBMC->UnknownToUTF8("Unknown"); */

			/* likes */
			entry.iStarRating = atoi(epgData.get("likes", "0").asString().c_str());

			XBMC->Log(LOG_DEBUG, "loaded EPG entry '%s' channel '%d' start '%d' end '%d'", entry.strTitle.c_str(), entry.iChannelId, entry.startTime, entry.endTime);

			EPG_TAG tag;
			memset(&tag, 0, sizeof(EPG_TAG));

			tag.iUniqueBroadcastId = entry.iBroadcastId + iAddBroadcastId;
			tag.strTitle = XBMC->UnknownToUTF8(entry.strTitle.c_str());
			tag.iChannelNumber = entry.iChannelId;
			tag.startTime = entry.startTime;
			tag.endTime = entry.endTime;
			tag.strPlot = entry.strPlot.c_str();
			tag.strIconPath = myChannel.strIconPath.c_str();
			tag.iGenreType = entry.iGenreType;
			tag.iGenreSubType = entry.iGenreSubType;
			//tag.strGenreDescription = entry.strGenreDescription.c_str();
			tag.iStarRating = entry.iStarRating;

			iLastEndTimeTmp = tag.endTime;

			PVR->TransferEpgEntry(handle, &tag);
		}

		iLastEndTime = iLastEndTimeTmp;
		XBMC->Log(LOG_DEBUG, "loaded EPG all entry %d", epg.size());
		iAddBroadcastId += epg.size();
	}
	return PVR_ERROR_NO_ERROR;
}
