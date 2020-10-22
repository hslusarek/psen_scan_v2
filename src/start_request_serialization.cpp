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
static constexpr std::size_t START_REQUEST_SIZE{ 58 };  // See protocol description
uint64_t const RESERVED_{ 0 };                          /**< Use all zeros */
uint32_t const OPCODE_{ htole32(0x35) };                /**< Constant 0x35. */

DynamicSizeRawData convertStringToDynamicSizeRawData(const std::string str)
{
  DynamicSizeRawData raw;
  raw.reserve(str.length());
  std::copy(str.begin(), str.end(), std::back_inserter(raw));
  return raw;
};

uint32_t calculateCRC(const DynamicSizeRawData& data)
{
  boost::crc_32_type crc;
  crc.process_bytes(&data.at(0), data.size());
  return static_cast<uint32_t>(crc.checksum());
}

DynamicSizeRawData serialize(const StartRequest& frame)
{
  std::ostringstream os;

  raw_processing::write(os, frame.seq_number_);
  raw_processing::write(os, RESERVED_);
  raw_processing::write(os, OPCODE_);

  /**< Byte order: big endian */
  uint32_t host_ip_big_endian = htobe32(frame.host_ip_);
  raw_processing::write(os, host_ip_big_endian);

  /**< Byte order: big endian */
  raw_processing::write(os, frame.host_udp_port_data_);

  /**< The following 'enable' fields are a 1-byte mask each.
   * Only the last 4 bits (little endian) are used, each of which represents a device.
   * For example, (1000) only enables the Master device, while (1010) enables both the Master
   * and the second Slave device.
   */

  uint8_t device_enabled{ 0 };
  uint8_t intensity_enabled{ 0 };
  uint8_t point_in_safety_enabled{ 0 };
  uint8_t active_zone_set_enabled{ 0 };
  uint8_t io_pin_enabled{ 0 };
  uint8_t scan_counter_enabled{ 0b00001000 };
  uint8_t speed_encoder_enabled{ 0 }; /**< 0000000bin disabled, 00001111bin enabled.*/
  uint8_t diagnostics_enabled{ 0b00000000 };

  // TODO:
  // Implement this lookup for the rest of the boolean status variables
  // Maybe write a getBitMask function.
  if (frame.device_settings_master_.getDeviceEnabled())
  {
    device_enabled += 0b00001000;
  }

  if (frame.device_settings_master_.getDiagnosticsEnabled())
  {
    diagnostics_enabled += 0b00001000;
  }

  raw_processing::write(os, device_enabled);
  raw_processing::write(os, intensity_enabled);
  raw_processing::write(os, point_in_safety_enabled);
  raw_processing::write(os, active_zone_set_enabled);
  raw_processing::write(os, io_pin_enabled);
  raw_processing::write(os, scan_counter_enabled);
  raw_processing::write(os, speed_encoder_enabled);
  raw_processing::write(os, diagnostics_enabled);

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

  DynamicSizeRawData raw_data = convertStringToDynamicSizeRawData(os.str());

  std::ostringstream os_crc;
  raw_processing::write(os_crc, calculateCRC(raw_data));

  std::string with_crc(os_crc.str() + os.str());
  DynamicSizeRawData raw_data_with_crc = convertStringToDynamicSizeRawData(with_crc);

  assert(with_crc.length() == START_REQUEST_SIZE && "Message data of start request has not the expected size");

  return raw_data_with_crc;
}
}  // namespace psen_scan_v2
