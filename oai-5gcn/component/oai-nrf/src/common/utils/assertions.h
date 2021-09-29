/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <inttypes.h>

#include "backtrace.h"

#ifndef ASSERTIONS_H_
#define ASSERTIONS_H_

#define _Assert_Exit_                                                          \
  {                                                                            \
    fprintf(stderr, "\nExiting execution\n");                                  \
    display_backtrace();                                                       \
    fflush(stdout);                                                            \
    fflush(stderr);                                                            \
    exit(EXIT_FAILURE);                                                        \
  }

#define _Assert_SegFault_                                                      \
  {                                                                            \
    fprintf(                                                                   \
        stderr,                                                                \
        "\n Will Intentionaly raise SEGFAULT to be catched by GDB!\n");        \
    display_backtrace();                                                       \
    fflush(stdout);                                                            \
    fflush(stderr);                                                            \
    *(int*) 0 = 0;                                                             \
    exit(EXIT_FAILURE);                                                        \
  }

#define _Assert_(cOND, aCTION, fORMAT, aRGS...)                                \
  do {                                                                         \
    if (!(cOND)) {                                                             \
      fprintf(                                                                 \
          stderr,                                                              \
          "\nAssertion (" #cOND                                                \
          ") failed!\n"                                                        \
          "In %s() %s:%d\n" fORMAT,                                            \
          __FUNCTION__, __FILE__, __LINE__, ##aRGS);                           \
      aCTION;                                                                  \
    }                                                                          \
  } while (0)

#if DEBUG_IS_ON
#define _ASSERT_FINAL_ _Assert_SegFault_
#else
#define _ASSERT_FINAL_ _Assert_Exit_
#endif

#define AssertFatal(cOND, ...) _Assert_(cOND, _ASSERT_FINAL_, ##__VA_ARGS__)
#define DevCheck(cOND, vALUE1, vALUE2, vALUE3)                                 \
  _Assert_(                                                                    \
      cOND, _ASSERT_FINAL_,                                                    \
      #vALUE1 ": %" PRIdMAX "\n" #vALUE2 ": %" PRIdMAX "\n" #vALUE3            \
              ": %" PRIdMAX "\n\n",                                            \
      (intmax_t) vALUE1, (intmax_t) vALUE2, (intmax_t) vALUE3)

#define DevCheck4(cOND, vALUE1, vALUE2, vALUE3, vALUE4)                        \
  _Assert_(                                                                    \
      cOND, _ASSERT_FINAL_,                                                    \
      #vALUE1 ": %" PRIdMAX "\n" #vALUE2 ": %" PRIdMAX "\n" #vALUE3            \
              ": %" PRIdMAX "\n" #vALUE4 ": %" PRIdMAX "\n\n",                 \
      (intmax_t) vALUE1, (intmax_t) vALUE2, (intmax_t) vALUE3,                 \
      (intmax_t) vALUE4)

#define DevAssert(cOND) _Assert_(cOND, _ASSERT_FINAL_, "")

#define DevMessage(mESSAGE) _Assert_(0, _ASSERT_FINAL_, #mESSAGE)

#define CHECK_INIT_RETURN(fCT)                                                 \
  do {                                                                         \
    int fct_ret;                                                               \
    if ((fct_ret = (fCT)) != 0) {                                              \
      fprintf(                                                                 \
          stderr,                                                              \
          "Function " #fCT                                                     \
          " has failed\n"                                                      \
          "returning %d\n",                                                    \
          fct_ret);                                                            \
      fflush(stdout);                                                          \
      fflush(stderr);                                                          \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

#define AssertError(cOND, aCTION, fORMAT, aRGS...)                             \
  _Assert_(cOND, aCTION, fORMAT, ##aRGS)
#define DevParam(vALUE1, vALUE2, vALUE3) DevCheck(0, vALUE1, vALUE2, vALUE3)

#endif /* ASSERTIONS_H_ */
