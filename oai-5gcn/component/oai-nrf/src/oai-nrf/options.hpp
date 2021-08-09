/*
 * Copyright (c) 2017 Sprint
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __OPTIONS_H
#define __OPTIONS_H

#include <stdint.h>
#include <string>

class Options {
 public:
  static bool parse(int argc, char** argv);
  static bool parseInputOptions(int argc, char** argv);
  static bool parseJson();
  static bool validateOptions();

  static const std::string& getlibconfigConfig() { return m_libconfigcfg; }
  static const bool& getlogRotFilelog() { return m_log_rot_file_log; }
  static const bool& getlogStdout() { return m_log_stdout; }

 private:
  enum OptionsSelected {
    libconfigcfg     = 0x01,
    log_stdout       = 0x02,
    log_rot_file_log = 0x04
  };

  static void help();

  static int options;

  static bool m_log_rot_file_log;
  static bool m_log_stdout;
  static std::string m_libconfigcfg;
};

#endif  // #define __OPTIONS_H
