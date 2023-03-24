/******************************************************************************
*  Filename:       data_packet.h
*
*  Description:    Defines for the data packet format.
*
* Copyright (C) 2017-2018 Texas Instruments Incorporated - http://www.ti.com/
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/


#include <stdint.h>

/*
 * Original Contents of general_packet.h ============
 */
#ifndef GENERAL_PACKET_H
#define GENERAL_PACKET_H

// General host interface packet format
#define SOF_LENGTH                              2
#define PACKET_TYPE_LENGTH                      1
#define PACKET_LEN_LENGTH                       2
#define MAX_PAYLOAD_LENGTH                      2047
#define EOF_LENGTH                              2
#define PACKET_NON_PAYLOAD_LENGTH               (SOF_LENGTH + PACKET_TYPE_LENGTH + PACKET_LEN_LENGTH + TIMESTAMP_LENGHT + EOF_LENGTH)
#define START_OF_FRAME_DELIMITER                0x5340
#define END_OF_FRAME_DELIMITER                  0x4540


// General packet header
#pragma pack(1)
typedef struct GeneralPacketHeader_Obj
{
    uint16_t sof;
    uint8_t packetType;
    uint16_t packetLen;
} GeneralPacketHeader_Obj;


// General packet footer
#pragma pack(1)
typedef struct GeneralPacketFooter_Obj
{
  uint16_t eof;
} GeneralPacketFooter_Obj;


/*
 * End of general_packet.h =========================
 */


// Data and error packet formats
#define MAX_DATA_PAYLOAD_LENGTH                         2047
#define MAX_EXTRA_RADIO_ADDED_BYTES                     10      // Up to 10 extra bytes are added by the radio (Timestamp, CRC, RSSI and Status fields)
#define TIMESTAMP_LENGHT                                6
#define ERROR_REPORT_PAYLOAD_LENGTH                     1

// Packet types
#define PACKET_TYPE_DATA                                0xC0    // Data packet
#define PACKET_TYPE_ERROR                               0xC1    // Error packet

// Data packet
#pragma pack(1)
typedef struct DataPacket_Obj
{
    GeneralPacketHeader_Obj genPacketHeader;
    uint8_t timeStamp[TIMESTAMP_LENGHT];
    uint8_t payload[MAX_DATA_PAYLOAD_LENGTH + MAX_EXTRA_RADIO_ADDED_BYTES + sizeof(GeneralPacketFooter_Obj)];     // space for payload and general packet footer
} DataPacket_Obj;

// Error packet
#pragma pack(1)
typedef struct ErrorReportPacket_Obj
{
    GeneralPacketHeader_Obj genPacketHeader;
    uint8_t error[ERROR_REPORT_PAYLOAD_LENGTH];
    GeneralPacketFooter_Obj genPacketFooter;
} ErrorReportPacket_Obj;

#endif

