/******************************************************************************
	Copyright (C) 2020 by Jérôme Leclercq <lynix680@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#pragma once

#ifndef OBS_KINECT_PLUGIN_WIN32HELPER
#define OBS_KINECT_PLUGIN_WIN32HELPER

#include <memory>
#include <type_traits>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

struct CloseHandleDeleter
{
	void operator()(HANDLE handle) const
	{
		if (handle != INVALID_HANDLE_VALUE)
			CloseHandle(handle);
	}
};

using HandlePtr = std::unique_ptr<std::remove_pointer_t<HANDLE>, CloseHandleDeleter>;

template<typename Interface>
struct CloseDeleter
{
	void operator()(Interface* handle) const
	{
		handle->Close();
	}
};

template<typename Interface>
struct ReleaseDeleter
{
	void operator()(Interface* handle) const
	{
		handle->Release();
	}
};

template<typename T> using ClosePtr = std::unique_ptr<T, CloseDeleter<T>>;
template<typename T> using ReleasePtr = std::unique_ptr<T, ReleaseDeleter<T>>;

// Kinect v1 sensor deleter (after NuiInitialize call)
template<typename NuiSensor>
struct SensorDeleter
{
	void operator()(NuiSensor* handle) const
	{
		handle->NuiShutdown();
	}
};

template<typename T> using InitializedNuiSensorPtr = std::unique_ptr<T, SensorDeleter<T>>;

#endif
