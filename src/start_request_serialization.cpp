// Copyright (c) 2020 Pilz GmbH & Co. KG
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <boost/crc.hpp>

#include <iostream>

#include "psen_scan_v2/start_request.h"
#include "psen_scan_v2/start_request_serialization.h"
#include "psen_scan_v2/raw_processing.h"

namespace psen_scan_v2
{
DynamicSizeRawData serialize(const StartRequest& frame)
{
  std::ostringstream os;

  raw_processing::write(os, frame.seq_number_);
  raw_processing::write(os, frame.RESERVED_);
  raw_processing::write(os, frame.OPCODE_);

  uint32_t host_ip_big_endian = htobe32(frame.host_ip_);
  raw_processing::write(os, host_ip_big_endian);

  raw_processing::write(os, frame.host_udp_port_data_);
  raw_processing::write(os, frame.device_enabled_);
  raw_processing::write(os, frame.intensity_enabled_);
  raw_processing::write(os, frame.point_in_safety_enabled_);
  raw_processing::write(os, frame.active_zone_set_enabled_);
  raw_processing::write(os, frame.io_pin_enabled_);
  raw_processing::write(os, frame.scan_counter_enabled_);
  raw_processing::write(os, frame.speed_encoder_enabled_);
  raw_processing::write(os, frame.diagnostics_enabled_);

  uint16_t start_angle{ frame.master_.getScanRange().getStart().value() };
  uint16_t end_angle{ frame.master_.getScanRange().getEnd().value() };
  uint16_t resolution{ frame.master_.getResolution().value() };
  raw_processing::write(os, start_angle);
  raw_processing::write(os, end_angle);
  raw_processing::write(os, resolution);

  for (const auto& slave : frame.slaves_)
  {
    uint16_t slave_start_angle{ slave.getScanRange().getStart().value() };
    uint16_t slave_end_angle{ slave.getScanRange().getEnd().value() };
    uint16_t slave_resolution{ slave.getResolution().value() };
    raw_processing::write(os, slave_start_angle);
    raw_processing::write(os, slave_end_angle);
    raw_processing::write(os, slave_resolution);
  }

  std::string data_str(os.str());
  DynamicSizeRawData raw_data;
  raw_data.reserve(data_str.length());
  std::copy(data_str.begin(), data_str.end(), std::back_inserter(raw_data));

  boost::crc_32_type crc;
  crc.process_bytes(&raw_data.at(0), raw_data.size());

  std::ostringstream os_crc;
  raw_processing::write(os_crc, static_cast<uint32_t>(crc.checksum()));
  std::string data_with_crc(os_crc.str() + os.str());

  DynamicSizeRawData raw_data_with_crc;
  raw_data_with_crc.reserve(data_with_crc.length());
  std::copy(data_with_crc.begin(), data_with_crc.end(), std::back_inserter(raw_data_with_crc));

  assert(data_with_crc.length() == START_REQUEST_SIZE && "Message data of start request has not the expected size");

  return raw_data_with_crc;
}
}  // namespace psen_scan_v2
