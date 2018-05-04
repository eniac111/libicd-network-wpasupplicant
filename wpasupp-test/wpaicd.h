/***********************************************************************************
*  libicd-network-wpasupplicant: Open source implementation of wpa_supplicant
*  integration icd2 on Maemo Leste
*  Copyright (C) 2018 Merlijn B. W. Wajer
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
***********************************************************************************/

#ifndef _ICDWPA_H_
#define _ICDWPA_H_
#include <gio/gio.h>

#include "wpasupplicant-defs.h"

#if 0
#define _WPA_ICD_DEBUG
#endif


/*
KeyMgmt as  Key management suite. Possible array elements: "wpa-psk", "wpa-eap", "wpa-none"
Pairwise    as  Pairwise cipher suites. Possible array elements: "ccmp", "tkip"
Group   s   Group cipher suite. Possible values are: "ccmp", "tkip", "wep104", "wep40" 
*/
typedef struct wpa {
    /* TODO/XXX: Yes, this all fits in a single bitfield, and yes, it should be
     * in a bitfield eventually, but not now. */

    /* Array */
    gboolean keymgmt_wpa_psk;
    gboolean keymgmt_wpa_eap;
    gboolean keymgmt_wpa_none;
    
    /* Array */
    gboolean pairwise_ccmp;
    gboolean pairwise_tkip;

    /* One of */
    gboolean group_ccmp;
    gboolean group_tkip;
    gboolean group_wep104;
    gboolean group_wep40;
} WpaInfo;

/*
KeyMgmt as  Key management suite. Possible array elements: "wpa-psk", "wpa-eap", "wpa-ft-psk", "wpa-ft-eap", "wpa-psk-sha256", "wpa-eap-sha256",
Pairwise    as  Pairwise cipher suites. Possible array elements: "ccmp", "tkip"
Group   s   Group cipher suite. Possible values are: "ccmp", "tkip", "wep104", "wep40"
MgmtGroup   s   Mangement frames cipher suite. Possible values are: "aes128cmac" 
*/
typedef struct rsn {
    /* TODO/XXX: Yes, this all fits in a single bitfield, and yes, it should be
     * in a bitfield eventually, but not now. */

    /* Array */
    gboolean keymgmt_wpa_psk;
    gboolean keymgmt_wpa_eap;
    gboolean keymgmt_wpa_ft_psk;
    gboolean keymgmt_wpa_ft_eap;
    gboolean keymgmt_wpa_psk_sha256;
    gboolean keymgmt_wpa_eap_sha256;

    /* Array */
    gboolean pairwise_ccmp;
    gboolean pairwise_tkip;

    /* One of */
    gboolean group_ccmp;
    gboolean group_tkip;
    gboolean group_wep104;
    gboolean group_wep40;

    /* One of */
    gboolean mgmtgroup_aes128cmac;
} RsnInfo;

typedef struct {
    guint16 frequency;

    /* True if infrastructure, False if ad-hoc */
    gboolean infrastructure;

    /* icd2 network_name */
    gchar* ssid;
    gsize ssid_len;

    /* TODO: network_attrs */

    gint16 signal;

    /* icd2 station_id */
    gchar* mac_addr;
    gsize mac_addr_len;

    WpaInfo wpa;
    RsnInfo rsn;
} BssInfo;

typedef void NetworkAdded(BssInfo*, void*);
typedef void ScanDone(int, void*);

#define _BSS_SIMPLE_INFO_FROM_DICT(gvar, keyname, structname, keytype, keytype2) \
{ \
    GVariant* var = g_variant_dict_lookup_value(gvar, keyname, keytype); \
    g_variant_get(var, keytype2, structname); \
    g_variant_unref(var); \
}

#define _BSS_BYTESTRING_FROM_DICT(gvar, keyname, target, target_len) \
{ \
    GVariant* var = g_variant_dict_lookup_value(gvar, keyname, G_VARIANT_TYPE_BYTESTRING); \
    GBytes* gb = g_variant_get_data_as_bytes(var); \
    const char* data = g_bytes_get_data(gb, &target_len); \
    target = malloc(sizeof(char)*target_len); \
    memcpy(target, data, sizeof(char)*target_len); \
    g_variant_unref(var); \
}

#define _MATCH_SET(s1, s2, setv) \
{ \
    if (strncmp(s1, s2, strlen(s2)) == 0) { \
        setv = 1; \
    } \
}

int wpaicd_init(void);
void wpaicd_free(void);
int wpaicd_initiate_scan(void);
void wpaicd_set_network_added_cb(NetworkAdded*, void*);
void wpaicd_set_scan_done_cb(ScanDone*, void*);

#endif /* _ICDWPA_H_ */