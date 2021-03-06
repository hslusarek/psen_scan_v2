// Copyright (c) 2019-2020 Pilz GmbH & Co. KG
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

#ifndef PSEN_SCAN_V2_MOCK_CONSOLE_BRIDGE_OUTPUT_HANDLER_H
#define PSEN_SCAN_V2_MOCK_CONSOLE_BRIDGE_OUTPUT_HANDLER_H

#include <gmock/gmock.h>
#include <console_bridge/console.h>

namespace psen_scan_v2_test
{
class MockConsoleBridgeOutputHandler : public console_bridge::OutputHandler
{
public:
  MOCK_METHOD4(log, void(const std::string&, console_bridge::LogLevel, const char*, int));
};

#define INJECT_LOG_MOCK                                                                                                \
  MockConsoleBridgeOutputHandler mock;                                                                                 \
  console_bridge::useOutputHandler(&mock);

#define EXPECT_LOG(level, msg, file, line)                                                                             \
  EXPECT_CALL(mock, log(msg, console_bridge::CONSOLE_BRIDGE_LOG_##level, file, line))

}  // namespace psen_scan_v2_test

#endif  // PSEN_SCAN_V2_MOCK_CONSOLE_BRIDGE_OUTPUT_HANDLER_H
