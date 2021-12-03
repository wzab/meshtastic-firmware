/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.4 */

#ifndef PB_DEVICEONLY_PB_H_INCLUDED
#define PB_DEVICEONLY_PB_H_INCLUDED
#include <pb.h>
#include "channel.pb.h"
#include "mesh.pb.h"
#include "radioconfig.pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _ChannelFile {
    pb_size_t channels_count;
    Channel channels[8];
} ChannelFile;

typedef struct _LegacyRadioConfig_LegacyPreferences {
    RegionCode region;
} LegacyRadioConfig_LegacyPreferences;

typedef struct _LegacyRadioConfig {
    bool has_preferences;
    LegacyRadioConfig_LegacyPreferences preferences;
} LegacyRadioConfig;

typedef struct _DeviceState {
    bool has_legacyRadio;
    LegacyRadioConfig legacyRadio;
    bool has_my_node;
    MyNodeInfo my_node;
    bool has_owner;
    User owner;
    pb_size_t node_db_count;
    NodeInfo node_db[32];
    pb_size_t receive_queue_count;
    MeshPacket receive_queue[1];
    bool has_rx_text_message;
    MeshPacket rx_text_message;
    uint32_t version;
    bool no_save;
    bool did_gps_reset;
} DeviceState;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define LegacyRadioConfig_init_default           {false, LegacyRadioConfig_LegacyPreferences_init_default}
#define LegacyRadioConfig_LegacyPreferences_init_default {_RegionCode_MIN}
#define DeviceState_init_default                 {false, LegacyRadioConfig_init_default, false, MyNodeInfo_init_default, false, User_init_default, 0, {NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default, NodeInfo_init_default}, 0, {MeshPacket_init_default}, false, MeshPacket_init_default, 0, 0, 0}
#define ChannelFile_init_default                 {0, {Channel_init_default, Channel_init_default, Channel_init_default, Channel_init_default, Channel_init_default, Channel_init_default, Channel_init_default, Channel_init_default}}
#define LegacyRadioConfig_init_zero              {false, LegacyRadioConfig_LegacyPreferences_init_zero}
#define LegacyRadioConfig_LegacyPreferences_init_zero {_RegionCode_MIN}
#define DeviceState_init_zero                    {false, LegacyRadioConfig_init_zero, false, MyNodeInfo_init_zero, false, User_init_zero, 0, {NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero, NodeInfo_init_zero}, 0, {MeshPacket_init_zero}, false, MeshPacket_init_zero, 0, 0, 0}
#define ChannelFile_init_zero                    {0, {Channel_init_zero, Channel_init_zero, Channel_init_zero, Channel_init_zero, Channel_init_zero, Channel_init_zero, Channel_init_zero, Channel_init_zero}}

/* Field tags (for use in manual encoding/decoding) */
#define ChannelFile_channels_tag                 1
#define LegacyRadioConfig_LegacyPreferences_region_tag 15
#define LegacyRadioConfig_preferences_tag        1
#define DeviceState_legacyRadio_tag              1
#define DeviceState_my_node_tag                  2
#define DeviceState_owner_tag                    3
#define DeviceState_node_db_tag                  4
#define DeviceState_receive_queue_tag            5
#define DeviceState_rx_text_message_tag          7
#define DeviceState_version_tag                  8
#define DeviceState_no_save_tag                  9
#define DeviceState_did_gps_reset_tag            11

/* Struct field encoding specification for nanopb */
#define LegacyRadioConfig_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, MESSAGE,  preferences,       1)
#define LegacyRadioConfig_CALLBACK NULL
#define LegacyRadioConfig_DEFAULT NULL
#define LegacyRadioConfig_preferences_MSGTYPE LegacyRadioConfig_LegacyPreferences

#define LegacyRadioConfig_LegacyPreferences_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    region,           15)
#define LegacyRadioConfig_LegacyPreferences_CALLBACK NULL
#define LegacyRadioConfig_LegacyPreferences_DEFAULT NULL

#define DeviceState_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, MESSAGE,  legacyRadio,       1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  my_node,           2) \
X(a, STATIC,   OPTIONAL, MESSAGE,  owner,             3) \
X(a, STATIC,   REPEATED, MESSAGE,  node_db,           4) \
X(a, STATIC,   REPEATED, MESSAGE,  receive_queue,     5) \
X(a, STATIC,   OPTIONAL, MESSAGE,  rx_text_message,   7) \
X(a, STATIC,   SINGULAR, UINT32,   version,           8) \
X(a, STATIC,   SINGULAR, BOOL,     no_save,           9) \
X(a, STATIC,   SINGULAR, BOOL,     did_gps_reset,    11)
#define DeviceState_CALLBACK NULL
#define DeviceState_DEFAULT NULL
#define DeviceState_legacyRadio_MSGTYPE LegacyRadioConfig
#define DeviceState_my_node_MSGTYPE MyNodeInfo
#define DeviceState_owner_MSGTYPE User
#define DeviceState_node_db_MSGTYPE NodeInfo
#define DeviceState_receive_queue_MSGTYPE MeshPacket
#define DeviceState_rx_text_message_MSGTYPE MeshPacket

#define ChannelFile_FIELDLIST(X, a) \
X(a, STATIC,   REPEATED, MESSAGE,  channels,          1)
#define ChannelFile_CALLBACK NULL
#define ChannelFile_DEFAULT NULL
#define ChannelFile_channels_MSGTYPE Channel

extern const pb_msgdesc_t LegacyRadioConfig_msg;
extern const pb_msgdesc_t LegacyRadioConfig_LegacyPreferences_msg;
extern const pb_msgdesc_t DeviceState_msg;
extern const pb_msgdesc_t ChannelFile_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define LegacyRadioConfig_fields &LegacyRadioConfig_msg
#define LegacyRadioConfig_LegacyPreferences_fields &LegacyRadioConfig_LegacyPreferences_msg
#define DeviceState_fields &DeviceState_msg
#define ChannelFile_fields &ChannelFile_msg

/* Maximum encoded size of messages (where known) */
#define LegacyRadioConfig_size                   4
#define LegacyRadioConfig_LegacyPreferences_size 2
#define DeviceState_size                         9594
#define ChannelFile_size                         832

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
