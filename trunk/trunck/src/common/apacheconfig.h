// Copyright 2008 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef COMMON_APACHECONFIG_H__
#define COMMON_APACHECONFIG_H__

#include <sys/types.h>
#include <string>
#include <vector>
#include <fstream>

#include "common/webserverconfig.h"

class ApacheConfig : public WebserverConfig {
 public:
  struct VHostConfig {
    // from ServerName, if this value not present,
    // and this vhost contains _default_ or * addresss,
    // ServerName for mainserver will be used as default
    std::string servername;
    
    // from DocumentRoot
    // if not present, DocumentRoot from mainserver is used as default
    std::string documentroot;
    
    // from <VirtualHost address1, [:address2...]>
    // So it may represent zero or more single address
    std::string address;
    
    // From CustomLog directive, represents the log path.
    std::string customlog;
  };

  ApacheConfig() {}
  virtual ~ApacheConfig() {}

  const std::string& group_name() const { return group_name_; }
  void set_group_name(const std::string& group_name) {
    group_name_ = group_name;
  }

  // Load configuration from IIS
  virtual bool Load();

  bool Load(const char* configfile);
  
  static void SetConfFilePath(const char* path) { conf_file_path_ = path; }
  static std::string GetConfFilePath() { return conf_file_path_; }

 private:
  // help functions
  static bool Readline(std::ifstream* fout, std::string* line);
  static std::vector<std::string> Split(const std::string& str);

  static std::string GetLocalHost();

  inline static bool IsSpace(char c) {
    return c == ' ' || c == '\t';
  }

  // default apache configuration path for Load() method.
  static std::string conf_file_path_;

  // Handle [ServerName] directive
  bool NormalizeServerName(std::string* servername);
  
  // process a configuration file
  bool ProcessFile(const char* filename);

  // Handle [Include] directive
  bool RetrieveFiles(const char* fname, std::vector<std::string>* files);

  // build site id from a vhost configuration
  bool BuildSiteId(VHostConfig* conf, std::string* siteid,
                   int defaultport);

  // make an absolute path from server name.
  bool MakeAbsolutePath(std::string* path);

  // default value: HTTPD_ROOT, ap_server_root
  // /usr/local/apache2
  std::string serverroot_;
  
  // default value for documentroot: DOCUMENT_LOCATION, "/htdocs"
  VHostConfig mainserver_;

  std::vector<VHostConfig> vhosts_;

  std::string group_name_;

};

#endif  // COMMON_APACHECONFIG_H__