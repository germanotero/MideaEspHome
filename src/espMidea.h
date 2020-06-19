/**
 * espmhp.h
 *
 * Header file for esphome-Midea
 *
 * Author: Geoff Davis.<geoff@geoffdavis.com>
 * Date: 2020-03-11
 * License: BSD
 *
 * Requirements:
 * - https://github.com/SwiCago/HeatPump
 * - ESPHome 1.15.0-dev or greater
 */

#define IR_EMITER       5

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <MideaIR.h>
#include "esphome.h"

using namespace esphome;

#ifndef ESPMHP_H
#define ESPMHP_H

static const char* TAG = "Midea"; // Logging tag

static const char* ESPMHP_VERSION = "1.2.0";

/* If polling interval is greater than 9 seconds, the HeatPump
library reconnects, but doesn't then follow up with our data request.*/
static const uint32_t ESPMHP_POLL_INTERVAL_DEFAULT = 500; // in milliseconds,
                                                           // 0 < X <= 9000
static const uint8_t ESPMHP_MIN_TEMPERATURE = 17; // degrees C,
                                                  // defined by hardware
static const uint8_t ESPMHP_MAX_TEMPERATURE = 30; // degrees C,
                                                  //defined by hardware
static const float   ESPMHP_TEMPERATURE_STEP = 1; // temperature setting step,
                                                    // in degrees C


class Midea : public PollingComponent, public climate::Climate {
    public:
        /**
         * Create a new Midea object
         *
         * Args:
         *   hw_serial: pointer to an Arduino HardwareSerial instance
         *   poll_interval: polling interval in milliseconds
         */
        Midea(
            uint32_t poll_interval=ESPMHP_POLL_INTERVAL_DEFAULT
        );

        // Print a banner with library information.
        void banner() {
            ESP_LOGI(TAG, "ESPHome Midea version %s",
                    ESPMHP_VERSION);
        }

        // print the current configuration
        void dump_config() override;

        // Set up the component, initializing the HeatPump object.
        void setup() override;

        // This is called every poll_interval.
        void update() override;

        // Configure the climate object with traits that we support.
        climate::ClimateTraits traits() override;

        // Debugging function to print the object's state.
        void dump_state();

        // Handle a request from the user to change settings.
        void control(const climate::ClimateCall &call) override;

};

#endif
