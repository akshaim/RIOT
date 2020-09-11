/*
 * Copyright (C) 2017 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    net_lora LoRa modulation
 * @ingroup     net
 * @brief       LoRa modulation header definitions
 * @{
 *
 * @file
 * @brief       LoRa modulation header definitions
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 */

#ifndef NET_LORA_H
#define NET_LORA_H

#include <stdint.h>
#include <stdlib.h>

#include "kernel_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup net_lora_conf  LoRa modulation compile configurations
 * @ingroup  config
 * @{
 */
/** @brief Frequency resolution in Hz */
#ifndef LORA_FREQUENCY_RESOLUTION_DEFAULT
#define LORA_FREQUENCY_RESOLUTION_DEFAULT      (61.03515625)
#endif

/** @brief Preamble length, same for Tx and Rx
 *
 * Configure preamble used in LoRa frame. Each LoRa frame begins with a
 * preamble. It starts with a series of upchirps to cover the whole frequency
 * band of the particular channel assigned. The last two upchirps encode the
 * sync word. Sync word is used to differntiate between LoRa transmissions that
 * use the same frequency bands. The sync word is followed by two and a quarter
 * downchirps, for a duration of 2.25 symbols. The total duration of this
 * preamble can be configured between 10.25 and 65,539.25 symbol hence the value
 * can range from 8 to 65537.
*/
#ifndef CONFIG_LORA_PREAMBLE_LENGTH_DEFAULT
#define CONFIG_LORA_PREAMBLE_LENGTH_DEFAULT         (8U)
#endif

/** @brief Symbols timeout (s) */
#ifndef CONFIG_LORA_SYMBOL_TIMEOUT_DEFAULT
#define CONFIG_LORA_SYMBOL_TIMEOUT_DEFAULT          (10U)
#endif

/** @brief Set channel bandwidth
 *
 * Configure the channel bandwidth. Refer to country specific regulation on
 * channel usage to identify the correct bandwidth. If LoRaWAN MAC is being
 * used, refer to "LoRaWAN Regional Parameters" by LoRa Alliance for more
 * information.
*/
#if IS_ACTIVE(CONFIG_LORA_BW_DEFAULT_125)
#define CONFIG_LORA_BW_DEFAULT                      (LORA_BW_125_KHZ)
#elif IS_ACTIVE(CONFIG_LORA_BW_DEFAULT_250)
#define CONFIG_LORA_BW_DEFAULT                      (LORA_BW_250_KHZ)
#elif IS_ACTIVE(CONFIG_LORA_BW_DEFAULT_500)
#define CONFIG_LORA_BW_DEFAULT                      (LORA_BW_500_KHZ)
#endif

#ifndef CONFIG_LORA_BW_DEFAULT
#define CONFIG_LORA_BW_DEFAULT                      (LORA_BW_125_KHZ)
#endif

/** @brief Set Spreading Factor (SF)
 *
 * Configure Spreading Factor (SF). SF denotes the amount of spreading code
 * applied to the original data signal. A larger SF increases the time on air,
 * which increases energy consumption, reduces the data rate, and improves
 * communication range. Each step up in spreading factor effectively doubles the
 * time on air to transmit the same amount of data. Refer to country specific
 * air time usage regulations before varying the SF. To calculate air time,in
 * the case of LoRa, refer https://www.loratools.nl/#/airtime . If LoRaWAN
 * is being used refer
 * https://avbentem.github.io/airtime-calculator/ttn/eu868/12,12
*/
#if IS_ACTIVE(CONFIG_LORA_SF_DEFAULT_SF6)
#define CONFIG_LORA_SF_DEFAULT                      (LORA_SF6)
#elif IS_ACTIVE(CONFIG_LORA_SF_DEFAULT_SF7)
#define CONFIG_LORA_SF_DEFAULT                      (LORA_SF7)
#elif IS_ACTIVE(CONFIG_LORA_SF_DEFAULT_SF8)
#define CONFIG_LORA_SF_DEFAULT                      (LORA_SF8)
#elif IS_ACTIVE(CONFIG_LORA_SF_DEFAULT_SF9)
#define CONFIG_LORA_SF_DEFAULT                      (LORA_SF9)
#elif IS_ACTIVE(CONFIG_LORA_SF_DEFAULT_SF10)
#define CONFIG_LORA_SF_DEFAULT                      (LORA_SF10)
#elif IS_ACTIVE(CONFIG_LORA_SF_DEFAULT_SF11)
#define CONFIG_LORA_SF_DEFAULT                      (LORA_SF11)
#elif IS_ACTIVE(CONFIG_LORA_SF_DEFAULT_SF12)
#define CONFIG_LORA_SF_DEFAULT                      (LORA_SF12)
#endif

#ifndef CONFIG_LORA_SF_DEFAULT
#define CONFIG_LORA_SF_DEFAULT                      (LORA_SF12)
#endif

/** @brief Set Coding Rate (CR)
 *
 * Configure Coding Rate (CR). CR denotes the implementation of forward error
 * correction (FEC). This may be done by encoding 4-bit data with redundancies
 * into 5-bit, 6-bit, 7-bit, or 8-bit. Coding Rate (CR) value need to be
 * adjusted according to conditions of the channel used for data transmission.
 * If there are too many interferences in the channel, then it’s recommended to
 * increase the value of CR. However, the rise in CR value will also increase
 * the duration for the transmission. Refer to country specific air time usage
 * regulations before varying the CR. To calculate air time,in the case of LoRa,
 * refer https://www.loratools.nl/#/airtime . If LoRaWAN MAC is being used
 * refer to "LoRaWAN Regional Parameters" by LoRa Alliance for more information.
*/
#if IS_ACTIVE(CONFIG_LORA_CR_DEFAULT_CR_4_5)
#define CONFIG_LORA_CR_DEFAULT                      (LORA_CR_4_5)
#elif IS_ACTIVE(CONFIG_LORA_CR_DEFAULT_CR_4_6)
#define CONFIG_LORA_CR_DEFAULT                      (LORA_CR_4_6)
#elif IS_ACTIVE(CONFIG_LORA_CR_DEFAULT_CR_4_7)
#define CONFIG_LORA_CR_DEFAULT                      (LORA_CR_4_7)
#elif IS_ACTIVE(CONFIG_LORA_CR_DEFAULT_CR_4_8)
#define CONFIG_LORA_CR_DEFAULT                      (LORA_CR_4_8)
#endif

#ifndef CONFIG_LORA_CR_DEFAULT
#define CONFIG_LORA_CR_DEFAULT                      (LORA_CR_4_8)
#endif

/** @brief Set this to true to enable inverted I/Q mode
 *
 * Inverted I/Q allows differentiating sender and receiver messages. In the case
 * of LoRaWAN, gateways receive messages with Inverted I/Q off and send messages
 * with Inverted I/Q on. Nodes read messages with Inverted I/Q on and send
 * messages with Inverted I/Q off. Set this to true if the device is to be used
 * as a gateway.
*/
#ifdef DOXYGEN
#define CONFIG_LORA_IQ_INVERTED_DEFAULT
#endif

/** @brief Set this to true to enable frequency hopping
 *
 * If Frequency hopping spread spectrum (FHSS) is enabled a portion of each LoRa
 * packet is transmitted on each hopping channel from a look up table of
 * frequencies managed by the host microcontroller.
*/
#ifdef DOXYGEN
#define CONFIG_LORA_FREQUENCY_HOPPING_DEFAULT
#endif

/** @brief Frequency hopping period
 *
 * Configure the hopping period, time which each transmission will dwell in any
 * given channel.
*/
#ifndef CONFIG_LORA_FREQUENCY_HOPPING_PERIOD_DEFAULT
#define CONFIG_LORA_FREQUENCY_HOPPING_PERIOD_DEFAULT (0U)
#endif

/** @brief Set this to true to enable fixed header length mode (implicit header)
 *
 * If fixed header length mode ( implicit header mode) is enabled, PHY header
 * (PHDR) in LoRa frame is discarded. The mode is typically used in LoRaWAN
 * beacons.
*/
#ifdef DOXYGEN
#define CONFIG_LORA_FIXED_HEADER_LEN_MODE_DEFAULT
#endif

/** @brief Enable/disable payload CRC, optional
 *
 * @deprecated Use inverse @ref CONFIG_LORA_PAYLOAD_CRC_OFF_DEFAULT instead.
 * Will be removed after 2021.04 release.
*/
#ifndef LORA_PAYLOAD_CRC_ON_DEFAULT
#if IS_ACTIVE(CONFIG_LORA_PAYLOAD_CRC_OFF_DEFAULT)
#define LORA_PAYLOAD_CRC_ON_DEFAULT                 (false)
#else
#define LORA_PAYLOAD_CRC_ON_DEFAULT                 (true)
#endif
#endif

/** @brief Configure payload length
 *
 * Configure the length of payload. The mode is disabled when implicit header
 * mode ( @ref CONFIG_LORA_FIXED_HEADER_LEN_MODE_DEFAULT ) is enabled.
*/
#ifndef CONFIG_LORA_PAYLOAD_LENGTH_DEFAULT
#define CONFIG_LORA_PAYLOAD_LENGTH_DEFAULT          (0U)
#endif
/** @} */

/**
 * @name    LoRa syncword values for network types
 * @{
 */
#define LORA_SYNCWORD_PUBLIC           (0x34)  /**< Syncword used for public networks */
#define LORA_SYNCWORD_PRIVATE          (0x12)  /**< Syncword used for private networks */
/** @} */

/**
 * @name    LoRa modulation available values
 *
 */
/**
 * @brief   LoRa modulation bandwidth.
 */
enum {
    LORA_BW_125_KHZ = 0,               /**< 125 kHz bandwidth */
    LORA_BW_250_KHZ,                   /**< 250 kHz bandwidth */
    LORA_BW_500_KHZ                    /**< 500 kHz bandwidth */
};

/**
 * @brief   LoRa modulation spreading factor rate
 */
enum {
    LORA_SF6 = 6,                      /**< spreading factor 6 */
    LORA_SF7,                          /**< spreading factor 7 */
    LORA_SF8,                          /**< spreading factor 8 */
    LORA_SF9,                          /**< spreading factor 9 */
    LORA_SF10,                         /**< spreading factor 10 */
    LORA_SF11,                         /**< spreading factor 11 */
    LORA_SF12                          /**< spreading factor 12 */
};

/**
 * @brief   LoRa modulation error coding rate.
 */
enum {
    LORA_CR_4_5 = 1,                   /**< coding rate 4/5 */
    LORA_CR_4_6,                       /**< coding rate 4/6 */
    LORA_CR_4_7,                       /**< coding rate 4/7 */
    LORA_CR_4_8                        /**< coding rate 4/8 */
};
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* NET_LORA_H */
/** @} */
