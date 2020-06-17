/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "context.h"

static udm_timer_cfg_t g_udm_timer_cfg[MAX_NUM_OF_UDM_TIMER] = {
    [UDM_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL] =
        { .duration = ogs_time_from_sec(3) },
    [UDM_TIMER_SBI_CLIENT_WAIT] =
        { .duration = ogs_time_from_msec(500) },
};

udm_timer_cfg_t *udm_timer_cfg(udm_timer_e id)
{
    ogs_assert(id < MAX_NUM_OF_UDM_TIMER);
    return &g_udm_timer_cfg[id];
}

const char *udm_timer_get_name(udm_timer_e id)
{
    switch (id) {
    case UDM_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        return "UDM_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL";
    case UDM_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        return "UDM_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL";
    case UDM_TIMER_NF_INSTANCE_HEARTBEAT:
        return "UDM_TIMER_NF_INSTANCE_HEARTBEAT";
    case UDM_TIMER_NF_INSTANCE_VALIDITY:
        return "UDM_TIMER_NF_INSTANCE_VALIDITY";
    case UDM_TIMER_SUBSCRIPTION_VALIDITY:
        return "UDM_TIMER_SUBSCRIPTION_VALIDITY";
    case UDM_TIMER_SBI_CLIENT_WAIT:
        return "UDM_TIMER_SBI_CLIENT_WAIT";
    default: 
       break;
    }

    return "UNKNOWN_TIMER";
}

static void sbi_timer_send_event(int timer_id, void *data)
{
    int rv;
    udm_event_t *e = NULL;
    ogs_assert(data);

    switch (timer_id) {
    case UDM_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
    case UDM_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
    case UDM_TIMER_NF_INSTANCE_HEARTBEAT:
    case UDM_TIMER_NF_INSTANCE_VALIDITY:
    case UDM_TIMER_SUBSCRIPTION_VALIDITY:
    case UDM_TIMER_SBI_CLIENT_WAIT:
        e = udm_event_new(UDM_EVT_SBI_TIMER);
        ogs_assert(e);
        e->timer_id = timer_id;
        e->sbi.data = data;
        break;
    default:
        ogs_fatal("Unknown timer id[%d]", timer_id);
        ogs_assert_if_reached();
        break;
    }

    rv = ogs_queue_push(udm_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed [%d] in %s",
                (int)rv, udm_timer_get_name(e->timer_id));
        udm_event_free(e);
    }
}

void udm_timer_nf_instance_registration_interval(void *data)
{
    sbi_timer_send_event(UDM_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL, data);
}

void udm_timer_nf_instance_heartbeat_interval(void *data)
{
    sbi_timer_send_event(UDM_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL, data);
}

void udm_timer_nf_instance_heartbeat(void *data)
{
    sbi_timer_send_event(UDM_TIMER_NF_INSTANCE_HEARTBEAT, data);
}

void udm_timer_nf_instance_validity(void *data)
{
    sbi_timer_send_event(UDM_TIMER_NF_INSTANCE_VALIDITY, data);
}

void udm_timer_subscription_validity(void *data)
{
    sbi_timer_send_event(UDM_TIMER_SUBSCRIPTION_VALIDITY, data);
}

void udm_timer_sbi_client_wait_expire(void *data)
{
    sbi_timer_send_event(UDM_TIMER_SBI_CLIENT_WAIT, data);
}
