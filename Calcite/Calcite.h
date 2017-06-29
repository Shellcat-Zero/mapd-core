/*
 * Copyright 2017 MapD Technologies, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * File:   Calcite.h
 * Author: michael
 *
 * Created on November 23, 2015, 9:33 AM
 */

#ifndef CALCITE_H
#define CALCITE_H

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <jni.h>
#include <thread>
#include "gen-cpp/CalciteServer.h"

class Calcite {
 public:
  Calcite(const int mapd_port, const int port, const std::string& data_dir, const size_t calcite_max_mem);
  std::string process(std::string user,
                      std::string passwd,
                      std::string catalog,
                      std::string sql_string,
                      const bool legacy_syntax,
                      const bool is_explain);
  std::string getExtensionFunctionWhitelist();
  void updateMetadata(std::string catalog, std::string table);
  virtual ~Calcite();

 private:
  void runJNI(const int port, const std::string& data_dir, const size_t calcite_max_memory);
  void runServer(const int mapd_port, const int port, const std::string& data_dir, const size_t calcite_max_mem);

  std::thread calcite_server_thread_;
  std::string handle_java_return(JNIEnv* env, jobject process_result);
  JNIEnv* checkJNIConnection();

  bool server_available_;
  bool jni_;
  int remote_calcite_port_ = -1;
  JavaVM* jvm_;
  jclass calciteDirect_;
  jobject calciteDirectObject_;
  jmethodID constructor_;
  jmethodID processMID_;
  jmethodID updateMetadataMID_;
  jmethodID hasFailedMID_;
  jmethodID getElapsedTimeMID_;
  jmethodID getTextMID_;
  jmethodID getExtensionFunctionWhitelistMID_;
};

#endif /* CALCITE_H */
