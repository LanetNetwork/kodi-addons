#pragma once
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

#include <vector>
#include "platform/util/StdString.h"
#include "client.h"

struct PVRLanetTVEpgEntry
{
	int				iBroadcastId;
	std::string		strTitle;
	int				iChannelId;
	time_t			startTime;
	time_t			endTime;
	std::string		strPlotOutline;
	std::string		strPlot;
	std::string		strIconPath;
	int				iGenreType;
	int				iGenreSubType;
	std::string		strGenreDescription;
	int				iStarRating;
};

struct PVRLanetTVChannel
{
	int								iUniqueId;
	int								iChannelNumber;
	int								iSubChannelNumber;
	int								iEncryptionSystem;
	std::string						strChannelName;
	std::string						strChannelEPG;
	std::string						strIconPath;
	std::string						strStreamURL;
};


class PVRLanetTVData
{
	public:
		PVRLanetTVData(void);
		virtual ~PVRLanetTVData(void);

		virtual PVR_ERROR GetChannels(ADDON_HANDLE handle, bool bRadio);
		virtual bool GetChannel(const PVR_CHANNEL &channel, PVRLanetTVChannel &myChannel);
		virtual int GetChannelsAmount(void);
	
		virtual PVR_ERROR GetEPGForChannel(ADDON_HANDLE handle, const PVR_CHANNEL &channel, time_t iStart, time_t iEnd);

	protected:
		virtual bool LoadChannelsData(void);

		virtual std::string LoadFile(std::string url);

	private:
		std::vector<PVRLanetTVChannel>	m_channels;
		time_t							m_iEpgStart;
		CStdString						m_strDefaultIcon;
		CStdString						m_strDefaultMovie;
		std::string						m_strListPack;
};
