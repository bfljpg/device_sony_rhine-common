/*
 * Copyright (C) 2018 Android Ice Cold Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// 0 = unknown
static int lastRilTech = 0;

// qcril_data_util_get_ril_tech_string gets called before dsi_get_current_data_bearer_tech,
// with the argument being the object of interest for us
const char* qcril_data_util_get_ril_tech_string(int rilTech) {
    // Remember RIL tech
    lastRilTech = rilTech;
    // Return arbitrary string: is only used for debugging
    return "RADIO_TECH_SHIM";
}

// [Hack] since this always returns 0 (BEARER_TECH_UNKNOWN) on rhine with shinano blobs,
// breaking more mobile networks than fixing, just return what makes
// qcril_data_validate_call_technology happy (that is: not abort data setup)
int dsi_get_current_data_bearer_tech(void* hndl) {
    int result = 0;
    if (       lastRilTech ==  6 // IS95A
            || lastRilTech ==  7 // IS95B
            || lastRilTech ==  8 // 1xRTT
            || lastRilTech ==  9 // EVDO_0
            || lastRilTech == 10 // EVDO_A
            || lastRilTech == 14) { // EVDO_B
        // qcril_data_validate_call_technology should be happy with FMC
        result = 6;
    } else if (lastRilTech ==  3 // GPRS
            || lastRilTech ==  4 // EDGE
            || lastRilTech == 11 // HSDPA
            || lastRilTech == 12 // HSUPA
            || lastRilTech == 13 // HSPA
            || lastRilTech == 15 // eHRPD
            || lastRilTech == 17 // HSPA+
            || lastRilTech == 19) { // TD_SCDMA
        // qcril_data_validate_call_technology should be happy with WCDMA
        result = 9;
    } // else: qcril_data_validate_call_technology should be happy with anything
    // Reset lastRilTech to unknown
    lastRilTech = 0;
    return result;
}
