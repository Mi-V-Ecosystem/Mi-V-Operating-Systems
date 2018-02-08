/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <mcu/cortex_m0.h>
#include "syscfg/syscfg.h"
#include "hal/hal_system.h"
#include <nrf51.h>
#include <nrf51_bitfields.h>

/**
 * Function called at startup. Called after BSS and .data initialized but
 * prior to the _start function.
 *
 * NOTE: this function is called by both the bootloader and the application.
 * If you add code here that you do not want executed in either case you need
 * to conditionally compile it using the config variable BOOT_LOADER (will
 * be set to 1 in case of bootloader build)
 *
 */
void
hal_system_init(void)
{
#if MYNEWT_VAL(MCU_DCDC_ENABLED)
    NRF_POWER->DCDCEN = 1;
#endif
}

void
hal_system_reset(void)
{
    while (1) {
        NVIC_SystemReset();
    }
}

int
hal_debugger_connected(void)
{
    /* XXX is there a way? */
    return 0;
}

/**
 * hal system clock start
 *
 * Makes sure the LFCLK and/or HFCLK is started.
 */
void
hal_system_clock_start(void)
{
    uint32_t mask;

#if MYNEWT_VAL(XTAL_32768)
    /* Check if this clock source is already running */
    mask = CLOCK_LFCLKSTAT_STATE_Msk | CLOCK_LFCLKSTAT_SRC_Xtal;
    if ((NRF_CLOCK->LFCLKSTAT & mask) != mask) {
        NRF_CLOCK->TASKS_LFCLKSTOP = 1;
        NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
        NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_Xtal;
        NRF_CLOCK->TASKS_LFCLKSTART = 1;

        /* Wait here till started! */
        while (1) {
            if (NRF_CLOCK->EVENTS_LFCLKSTARTED) {
                if ((NRF_CLOCK->LFCLKSTAT & mask) == mask) {
                    break;
                }
            }
        }
    }
#endif

#if MYNEWT_VAL(XTAL_32768_SYNTH)
    /* Must turn on HFLCK for synthesized 32768 crystal */
    mask = CLOCK_LFCLKSTAT_STATE_Msk | CLOCK_LFCLKSRC_SRC_Synth;
    if ((NRF_CLOCK->LFCLKSTAT & mask) != mask) {
        mask = CLOCK_HFCLKSTAT_STATE_Msk | CLOCK_HFCLKSTAT_SRC_Msk;
        if ((NRF_CLOCK->HFCLKSTAT & mask) != mask) {
            NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
            NRF_CLOCK->TASKS_HFCLKSTART = 1;
            while (1) {
                if ((NRF_CLOCK->EVENTS_HFCLKSTARTED) != 0) {
                    break;
                }
            }
        }

        NRF_CLOCK->TASKS_LFCLKSTOP = 1;
        NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
        NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_Synth;
        NRF_CLOCK->TASKS_LFCLKSTART = 1;
        while (1) {
            if (NRF_CLOCK->EVENTS_LFCLKSTARTED) {
                mask = CLOCK_LFCLKSTAT_STATE_Msk | CLOCK_LFCLKSRC_SRC_Synth;
                if ((NRF_CLOCK->LFCLKSTAT & mask) == mask) {
                    break;
                }
            }
        }
    }
#endif
}
