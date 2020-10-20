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

#ifndef PSEN_SCAN_V2_UDP_FRAME_DUMPS_H
#define PSEN_SCAN_V2_UDP_FRAME_DUMPS_H

#include <array>
#include <vector>
#include <map>
#include <iostream>

#include "psen_scan_v2/monitoring_frame_msg.h"

using namespace psen_scan_v2;

namespace psen_scan_v2_test
{
namespace scanner_udp_datagram_hexdumps
{
constexpr uint16_t convertHexdumpBytesToUint16_t(uint8_t msbyte, uint8_t lsbyte)
{
  return static_cast<uint16_t>(msbyte << 8) + static_cast<uint16_t>(lsbyte);
}

template <size_t ARRAY_SIZE>
inline std::vector<double>
readMeasures(const std::array<uint8_t, ARRAY_SIZE> hex_dump, const size_t offset_measures, const size_t n_measures)
{
  std::vector<double> measures;
  for (size_t idx = offset_measures; idx < (offset_measures + (n_measures * 2)); idx = idx + 2)
  {
    uint16_t raw_value = convertHexdumpBytesToUint16_t(hex_dump.at(idx + 1), hex_dump.at(idx));
    measures.push_back(raw_value / 1000.);
  }
  return measures;
}

class WithoutIntensities
{
public:
  WithoutIntensities()
  {
    monitoring_frame::MonitoringFrameMsg msg(
        TenthOfDegree(0x03e8), TenthOfDegree(0x0a), 0x00000678, readMeasures(hex_dump, 31, 50));
    expected_msg_ = msg;
  };

  // clang-format off
  const std::array<uint8_t, 135> hex_dump = {
                                                                0x00, 0x00, 0x00, 0x00, 0xca, 0x00,  // 0020
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x0a, 0x00, 0x02,  // 0030
    0x05, 0x00, 0x78, 0x06, 0x00, 0x00, 0x05, 0x65, 0x00, 0xa9, 0x02, 0x06, 0x03, 0x7c, 0x02, 0xfa,  // 0040
    0x01, 0xf0, 0x01, 0x0e, 0x02, 0x77, 0x01, 0xb4, 0x01, 0xd6, 0x01, 0xd8, 0x01, 0xe5, 0x01, 0xe1,  // 0050
    0x01, 0xd4, 0x01, 0xd1, 0x01, 0xc7, 0x01, 0xba, 0x01, 0xb9, 0x01, 0xaf, 0x01, 0xac, 0x01, 0xb5,  // 0060
    0x01, 0x90, 0x0c, 0xc2, 0x0c, 0xb8, 0x0c, 0xd6, 0x0c, 0xd5, 0x0d, 0xa7, 0x10, 0xb5, 0x0d, 0xc0,  // 0070
    0x0f, 0x45, 0x0e, 0xd2, 0x0f, 0xdf, 0x0f, 0xf7, 0x0e, 0x8d, 0x0e, 0xa9, 0x0e, 0x41, 0x09, 0x11,  // 0080
    0x09, 0x0e, 0x09, 0xe7, 0x08, 0xe7, 0x08, 0xdd, 0x08, 0xeb, 0x08, 0xeb, 0x08, 0xd4, 0x08, 0xf1,  // 0090
    0x08, 0x04, 0x09, 0xea, 0x0f, 0xea, 0x0f, 0x03, 0x10, 0xc6, 0x10, 0x31, 0x12, 0x09, 0x00, 0x00,  // 00a0
    0x00                                                                                             // 00b0
  };
  // clang-format on

  monitoring_frame::MonitoringFrameMsg expected_msg_;
};
class WithDiagnostics
{
public:
  WithDiagnostics()
  {
    monitoring_frame::MonitoringFrameMsg msg(
        TenthOfDegree(0x03e8),
        TenthOfDegree(0x01),
        0x000a6f10,
        readMeasures(hex_dump, 74, 500),
        { monitoring_frame::MonitoringFrameDiagnosticMessage(ScannerId::MASTER, monitoring_frame::ErrorLocation(1, 7)),
          monitoring_frame::MonitoringFrameDiagnosticMessage(ScannerId::MASTER,
                                                             monitoring_frame::ErrorLocation(4, 3)) });
    expected_msg_ = msg;
  };

  // clang-format off
  const std::array<uint8_t, 1078> hex_dump = {
                                                               0x00, 0x00, 0x00, 0x00, 0xca, 0x00,  //  offset:  0020
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x03, 0x01, 0x00, 0x02,  //  offset:  0030
   0x05, 0x00, 0x10, 0x6f, 0x0a, 0x00, 0x04, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,  //  offset:  0040
   0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //  offset:  0050
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //  offset:  0060
   0x00, 0x05, 0xe9, 0x03, 0xde, 0x01, 0xe0, 0x01, 0xd5, 0x01, 0xdc, 0x01, 0xd6, 0x01, 0xda, 0x01,  //  offset:  0070
   0xd8, 0x01, 0xd8, 0x01, 0xd9, 0x01, 0xe1, 0x01, 0xd7, 0x01, 0xd5, 0x01, 0xd9, 0x01, 0xcd, 0x01,  //  offset:  0080
   0xd0, 0x01, 0xcf, 0x01, 0xca, 0x01, 0xcf, 0x01, 0xcd, 0x01, 0xd2, 0x01, 0xc8, 0x01, 0xc9, 0x01,  //  offset:  0090
   0xcb, 0x01, 0xc8, 0x01, 0xd6, 0x01, 0xc0, 0x01, 0xd3, 0x01, 0xca, 0x01, 0xbc, 0x01, 0xc0, 0x01,  //  offset:  00a0
   0xbb, 0x01, 0xb7, 0x01, 0xc5, 0x01, 0xbe, 0x01, 0xbc, 0x01, 0xaf, 0x01, 0xb4, 0x01, 0xbd, 0x01,  //  offset:  00b0
   0xbc, 0x01, 0xbf, 0x01, 0xc0, 0x01, 0xc7, 0x01, 0xbf, 0x01, 0xc9, 0x01, 0xc3, 0x01, 0xc9, 0x01,  //  offset:  00c0
   0xc4, 0x01, 0xc7, 0x01, 0xc9, 0x01, 0xc5, 0x01, 0xcc, 0x01, 0xca, 0x01, 0xc7, 0x01, 0xc9, 0x01,  //  offset:  00d0
   0xc0, 0x01, 0xc9, 0x01, 0xc4, 0x01, 0xc3, 0x01, 0xc5, 0x01, 0xbf, 0x01, 0xc9, 0x01, 0xc4, 0x01,  //  offset:  00e0
   0xc4, 0x01, 0xbd, 0x01, 0xb3, 0x01, 0xb9, 0x01, 0xb5, 0x01, 0xb3, 0x01, 0xbd, 0x01, 0xb8, 0x01,  //  offset:  00f0
   0xb6, 0x01, 0xaf, 0x01, 0xbb, 0x01, 0xb7, 0x01, 0xb4, 0x01, 0xb7, 0x01, 0xba, 0x01, 0xc2, 0x01,  //  offset:  0100
   0xc4, 0x01, 0xcf, 0x01, 0xd3, 0x01, 0xd6, 0x01, 0xd0, 0x01, 0xe4, 0x01, 0x04, 0x02, 0x0d, 0x02,  //  offset:  0110
   0x5f, 0x02, 0x56, 0x02, 0x50, 0x07, 0x4a, 0x07, 0x48, 0x07, 0x4e, 0x07, 0x48, 0x07, 0x49, 0x07,  //  offset:  0120
   0x4a, 0x07, 0x45, 0x07, 0x45, 0x07, 0x45, 0x07, 0x48, 0x07, 0x45, 0x07, 0x41, 0x07, 0x3f, 0x07,  //  offset:  0130
   0x3f, 0x07, 0x3b, 0x07, 0x3d, 0x07, 0x39, 0x07, 0x3b, 0x07, 0x3a, 0x07, 0x3d, 0x07, 0x37, 0x07,  //  offset:  0140
   0x3a, 0x07, 0x39, 0x07, 0x33, 0x07, 0x38, 0x07, 0x33, 0x07, 0x33, 0x07, 0x33, 0x07, 0x35, 0x07,  //  offset:  0150
   0x34, 0x07, 0x36, 0x07, 0x2f, 0x07, 0x28, 0x07, 0x29, 0x07, 0x2b, 0x07, 0x2a, 0x07, 0x2b, 0x07,  //  offset:  0160
   0x2e, 0x07, 0x27, 0x07, 0x28, 0x07, 0x2a, 0x07, 0x23, 0x07, 0x29, 0x07, 0x2a, 0x07, 0x23, 0x07,  //  offset:  0170
   0x2a, 0x07, 0x2b, 0x07, 0x20, 0x07, 0x20, 0x07, 0x26, 0x07, 0x24, 0x07, 0x20, 0x07, 0x2a, 0x07,  //  offset:  0180
   0x1f, 0x07, 0x05, 0x07, 0x2a, 0x07, 0xce, 0x07, 0x5a, 0x08, 0x51, 0x08, 0x3d, 0x08, 0x80, 0x08,  //  offset:  0190
   0x71, 0x08, 0x7e, 0x08, 0x75, 0x08, 0x6d, 0x08, 0x76, 0x08, 0x7d, 0x08, 0x6b, 0x08, 0x70, 0x08,  //  offset:  01a0
   0x71, 0x08, 0x38, 0x08, 0xca, 0x07, 0x32, 0x07, 0x44, 0x07, 0x68, 0x07, 0x7a, 0x07, 0x8d, 0x07,  //  offset:  01b0
   0x95, 0x07, 0x95, 0x07, 0xb1, 0x07, 0xc4, 0x07, 0xd6, 0x07, 0xdf, 0x07, 0xe8, 0x07, 0xfa, 0x07,  //  offset:  01c0
   0x03, 0x08, 0x16, 0x08, 0x1f, 0x08, 0x32, 0x08, 0x3b, 0x08, 0x3b, 0x08, 0x57, 0x08, 0x57, 0x08,  //  offset:  01d0
   0x57, 0x08, 0x28, 0x08, 0x1f, 0x08, 0x5f, 0x08, 0x63, 0x08, 0x61, 0x08, 0x5d, 0x08, 0x5e, 0x08,  //  offset:  01e0
   0x5a, 0x08, 0x5e, 0x08, 0x57, 0x08, 0x58, 0x08, 0x5e, 0x08, 0x61, 0x08, 0x58, 0x08, 0x58, 0x08,  //  offset:  01f0
   0x56, 0x08, 0x5b, 0x08, 0x56, 0x08, 0x5a, 0x08, 0x52, 0x08, 0x51, 0x08, 0x56, 0x08, 0x57, 0x08,  //  offset:  0200
   0x57, 0x08, 0x53, 0x08, 0x54, 0x08, 0x4d, 0x08, 0x4d, 0x08, 0x53, 0x08, 0x54, 0x08, 0x53, 0x08,  //  offset:  0210
   0x4f, 0x08, 0x4b, 0x08, 0x4f, 0x08, 0x4d, 0x08, 0x4d, 0x08, 0x50, 0x08, 0x53, 0x08, 0x4a, 0x08,  //  offset:  0220
   0x54, 0x08, 0xf6, 0x07, 0xc8, 0x07, 0x88, 0x07, 0x6d, 0x07, 0x5b, 0x07, 0x5b, 0x07, 0x76, 0x07,  //  offset:  0230
   0x7f, 0x07, 0x91, 0x07, 0x9a, 0x07, 0x64, 0x07, 0x2d, 0x07, 0x4f, 0x07, 0x4f, 0x07, 0x4a, 0x07,  //  offset:  0240
   0x46, 0x07, 0x4f, 0x07, 0x48, 0x07, 0x49, 0x07, 0x4b, 0x07, 0x52, 0x07, 0x4d, 0x07, 0x38, 0x07,  //  offset:  0250
   0x41, 0x07, 0x36, 0x07, 0x3a, 0x07, 0x37, 0x07, 0x31, 0x07, 0x33, 0x07, 0x2e, 0x07, 0x19, 0x07,  //  offset:  0260
   0xf5, 0x06, 0xe7, 0x06, 0xe6, 0x06, 0xdc, 0x06, 0xe8, 0x06, 0xea, 0x06, 0xe3, 0x06, 0xdd, 0x06,  //  offset:  0270
   0xde, 0x06, 0xe0, 0x06, 0xe3, 0x06, 0xe5, 0x06, 0xdd, 0x06, 0xec, 0x06, 0xe9, 0x06, 0xea, 0x06,  //  offset:  0280
   0xef, 0x06, 0xfc, 0x06, 0x20, 0x07, 0x3d, 0x07, 0x43, 0x07, 0x57, 0x07, 0x3f, 0x07, 0x4d, 0x07,  //  offset:  0290
   0x52, 0x07, 0x45, 0x07, 0x4f, 0x07, 0x4e, 0x07, 0x47, 0x07, 0x43, 0x07, 0x4a, 0x07, 0x4a, 0x07,  //  offset:  02a0
   0x46, 0x07, 0x47, 0x07, 0x40, 0x07, 0x4b, 0x07, 0x44, 0x07, 0x48, 0x07, 0x46, 0x07, 0x40, 0x07,  //  offset:  02b0
   0x26, 0x07, 0xef, 0x06, 0xe2, 0x06, 0xd3, 0x01, 0xbf, 0x01, 0xaa, 0x01, 0x94, 0x01, 0x94, 0x01,  //  offset:  02c0
   0x7f, 0x01, 0x7f, 0x01, 0x74, 0x01, 0x69, 0x01, 0x53, 0x01, 0xfd, 0x01, 0x08, 0x02, 0xf2, 0x01,  //  offset:  02d0
   0xf3, 0x01, 0xe7, 0x01, 0xdd, 0x01, 0xdd, 0x01, 0xd2, 0x01, 0xc7, 0x01, 0xc7, 0x01, 0xbc, 0x01,  //  offset:  02e0
   0xbc, 0x01, 0xbc, 0x01, 0xb1, 0x01, 0xbc, 0x01, 0xb1, 0x01, 0xb1, 0x01, 0xb1, 0x01, 0xb1, 0x01,  //  offset:  02f0
   0xa6, 0x01, 0xb1, 0x01, 0xa6, 0x01, 0x9b, 0x01, 0x9b, 0x01, 0x9b, 0x01, 0x9b, 0x01, 0x9b, 0x01,  //  offset:  0300
   0x9b, 0x01, 0x9b, 0x01, 0x9b, 0x01, 0x9b, 0x01, 0x90, 0x01, 0x9b, 0x01, 0x90, 0x01, 0x9b, 0x01,  //  offset:  0310
   0xa6, 0x01, 0x9b, 0x01, 0x9b, 0x01, 0x90, 0x01, 0xc4, 0x02, 0xc3, 0x02, 0xc6, 0x02, 0xc2, 0x02,  //  offset:  0320
   0xc2, 0x02, 0xc5, 0x02, 0xc7, 0x02, 0xbd, 0x02, 0xc0, 0x02, 0xbf, 0x02, 0xbe, 0x02, 0xc4, 0x02,  //  offset:  0330
   0xc2, 0x02, 0xbe, 0x02, 0xbe, 0x02, 0xbc, 0x02, 0xbc, 0x02, 0xc1, 0x02, 0xc4, 0x02, 0xbd, 0x02,  //  offset:  0340
   0xc6, 0x02, 0xc0, 0x02, 0xbd, 0x02, 0xbb, 0x02, 0xc4, 0x02, 0xbf, 0x02, 0xbd, 0x02, 0xc0, 0x02,  //  offset:  0350
   0xbf, 0x02, 0xbf, 0x02, 0xc0, 0x02, 0xc1, 0x02, 0xc5, 0x02, 0xbf, 0x02, 0xbf, 0x02, 0xc0, 0x02,  //  offset:  0360
   0xc2, 0x02, 0xbf, 0x02, 0xc4, 0x02, 0xc2, 0x02, 0xc2, 0x02, 0xc7, 0x02, 0xcf, 0x02, 0xc9, 0x02,  //  offset:  0370
   0xc9, 0x02, 0xca, 0x02, 0xc9, 0x02, 0xc9, 0x02, 0xca, 0x02, 0xd3, 0x02, 0xcf, 0x02, 0xd4, 0x02,  //  offset:  0380
   0xd3, 0x02, 0xe7, 0x02, 0xcf, 0x02, 0xee, 0x02, 0xdb, 0x02, 0xeb, 0x02, 0xd2, 0x02, 0xdc, 0x02,  //  offset:  0390
   0xd0, 0x02, 0xd1, 0x02, 0xc8, 0x02, 0xd2, 0x02, 0xd2, 0x02, 0xda, 0x02, 0xcc, 0x02, 0xd2, 0x02,  //  offset:  03a0
   0xce, 0x02, 0xc9, 0x02, 0xcb, 0x02, 0xd1, 0x02, 0xc7, 0x02, 0xca, 0x02, 0xc8, 0x02, 0xcb, 0x02,  //  offset:  03b0
   0xc6, 0x02, 0xcb, 0x02, 0xc2, 0x02, 0xc4, 0x02, 0xc6, 0x02, 0xc2, 0x02, 0xc2, 0x02, 0xc9, 0x02,  //  offset:  03c0
   0xc3, 0x02, 0xc3, 0x02, 0xc5, 0x02, 0xc5, 0x02, 0xbd, 0x02, 0xc4, 0x02, 0xc1, 0x02, 0xc9, 0x02,  //  offset:  03d0
   0xbf, 0x02, 0xc5, 0x02, 0xbe, 0x02, 0xc1, 0x02, 0xc3, 0x02, 0xbe, 0x02, 0xbd, 0x02, 0xc1, 0x02,  //  offset:  03e0
   0xbd, 0x02, 0xba, 0x02, 0xbd, 0x02, 0xc1, 0x02, 0xbf, 0x02, 0xbd, 0x02, 0xbf, 0x02, 0xc0, 0x02,  //  offset:  03f0
   0xc0, 0x02, 0xc6, 0x02, 0xb8, 0x02, 0xbc, 0x02, 0xbe, 0x02, 0xbf, 0x02, 0xc0, 0x02, 0xc3, 0x02,  //  offset:  0400
   0xc2, 0x02, 0xc5, 0x02, 0xc5, 0x02, 0xc2, 0x02, 0xc6, 0x02, 0xc3, 0x02, 0xc8, 0x02, 0xc7, 0x02,  //  offset:  0410
   0xcc, 0x02, 0xd1, 0x02, 0xc5, 0x02, 0xcc, 0x02, 0xd2, 0x02, 0xce, 0x02, 0xd1, 0x02, 0xce, 0x02,  //  offset:  0420
   0xd3, 0x02, 0xd7, 0x02, 0xd7, 0x02, 0xd6, 0x02, 0xd7, 0x02, 0xd6, 0x02, 0xda, 0x02, 0xd7, 0x02,  //  offset:  0430
   0xdd, 0x02, 0xdc, 0x02, 0xde, 0x02, 0xdd, 0x02, 0xdb, 0x02, 0xda, 0x02, 0xdb, 0x02, 0xdf, 0x02,  //  offset:  0440
   0xe1, 0x02, 0xea, 0x02, 0xe4, 0x02, 0xe0, 0x02, 0xe0, 0x02, 0xe0, 0x02, 0x09, 0x00, 0x00, 0x00   //  offset:  0450
  };
  // clang-format on

  monitoring_frame::MonitoringFrameMsg expected_msg_;
};

class WithoutMeasurementsAndIntensities
{
public:
  WithoutMeasurementsAndIntensities()
  {
    monitoring_frame::MonitoringFrameMsg msg(TenthOfDegree(0x5dc), TenthOfDegree(0x0a), 0x0661fc, {});
    expected_msg_ = msg;
  }

  const std::array<uint8_t, 35> hex_dump = { 0x00, 0x00, 0x00, 0x00, 0xca, 0x00,  // 0020
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
                                             0x00, 0xdc, 0x05, 0x0a, 0x00, 0x02, 0x05, 0x00, 0xfc, 0x61,
                                             0x06, 0x00, 0x05, 0x01, 0x00, 0x09, 0x00, 0x00, 0x00 };

  monitoring_frame::MonitoringFrameMsg expected_msg_;
};

class WithUnknownFieldId
{
public:
  const std::array<uint8_t, 35> hex_dump = { 0x00, 0x00, 0x00, 0x00, 0xca, 0x00,  // 0020
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
                                             0x00, 0xdc, 0x05, 0x0a, 0x00, 0x02, 0x05, 0x00, 0xfc, 0x61,
                                             0x06, 0x00, 0x0a, 0x01, 0x00, 0x09, 0x00, 0x00, 0x00 };
};

class WithTooLargeFieldLength
{
public:
  const std::array<uint8_t, 35> hex_dump = { 0x00, 0x00, 0x00, 0x00, 0xca, 0x00,  // 0020
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
                                             0x00, 0xdc, 0x05, 0x0a, 0x00, 0x02, 0x05, 0x00, 0xfc, 0x61,
                                             0x06, 0x00, 0x05, 0xcf, 0xff, 0x09, 0x00, 0x00, 0x00 };
};

class WithTooLargeScanCounterLength
{
private:
  unsigned char modv{ 0x06 };

public:
  const std::array<uint8_t, 35> hex_dump = { 0x00, 0x00, 0x00, 0x00, 0xca, 0x00,  // 0020
                                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00,
                                             0x00, 0xdc, 0x05, 0x0a, 0x00, 0x02, modv, 0x00, 0xfc, 0x61,
                                             0x06, 0x00, 0x05, 0xcf, 0xff, 0x09, 0x00, 0x00, 0x00 };
};
}  // namespace scanner_udp_datagram_hexdumps
}  // namespace psen_scan_v2_test

#endif  // PSEN_SCAN_V2_UDP_FRAME_DUMPS_H
