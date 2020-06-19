/**
 * espmhp.cpp
 *
 * Implementation of esphome-Midea
 *
 * Author: Geoff Davis.<geoff@geoffdavis.com>
 * Author: Phil Genera @pgenera on Github.
 * Last Updated: 2020-06-02
 * License: BSD
 *
 * Requirements:
 * - https://github.com/SwiCago/HeatPump
 * - ESPHome 1.15.0-dev or greater
 */

#include "espMidea.h"

#define IR_EMITER       5

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <MideaIR.h>


IRsend irsend(IR_EMITER);
MideaIR remote_control(&irsend);


void Midea::update() {
    // This will be called every "update_interval" milliseconds.
    this->dump_config();
}

/**
 * Define our supported traits.
 *
 * Note:
 * Many of the following traits are only available in the 1.5.0 dev train of
 * ESPHome, particularly the Dry operation mode, and several of the fan modes.
 *
 * Returns:
 *   This class' supported climate::ClimateTraits.
 */
climate::ClimateTraits Midea::traits() {
    auto traits = climate::ClimateTraits();
    traits.set_supports_action(true);
    traits.set_supports_current_temperature(true);
    traits.set_supports_auto_mode(true);
    traits.set_supports_cool_mode(true);
    traits.set_supports_heat_mode(true);
    traits.set_supports_dry_mode(true);
    traits.set_supports_fan_only_mode(true);
    traits.set_supports_two_point_target_temperature(false);
    traits.set_supports_away(false);
    traits.set_visual_min_temperature(ESPMHP_MIN_TEMPERATURE);
    traits.set_visual_max_temperature(ESPMHP_MAX_TEMPERATURE);
    traits.set_visual_temperature_step(ESPMHP_TEMPERATURE_STEP);
    traits.set_supports_fan_mode_on(false);
    traits.set_supports_fan_mode_off(false);
    traits.set_supports_fan_mode_auto(true);
    traits.set_supports_fan_mode_focus(false);
    traits.set_supports_fan_mode_diffuse(false);
    traits.set_supports_fan_mode_low(true);
    traits.set_supports_fan_mode_medium(true);
    traits.set_supports_fan_mode_middle(false);
    traits.set_supports_fan_mode_high(true);
    traits.set_supports_swing_mode_off(false);
    traits.set_supports_swing_mode_both(false);
    traits.set_supports_swing_mode_vertical(false);
    traits.set_supports_swing_mode_horizontal(false);
    return traits;
}

/**
 * Implement control of a Midea.
 *
 * Maps HomeAssistant/ESPHome modes to Mitsubishi modes.
 */
void Midea::control(const climate::ClimateCall &call) {
    ESP_LOGV(TAG, "Control called.");

    bool updated = false;

    remote_control.turnON();
    if (call.get_mode().has_value()){
        this->mode = *call.get_mode();
        switch (*call.get_mode()) {
            case climate::CLIMATE_MODE_COOL:
                remote_control.setMode(mode_cool);
                updated = true;
                break;
            case climate::CLIMATE_MODE_HEAT:
                remote_control.setMode(mode_heat);
                updated = true;
                break;
            case climate::CLIMATE_MODE_DRY:
                remote_control.setMode(mode_no_humidity);
                updated = true;
                break;
            case climate::CLIMATE_MODE_AUTO:
                remote_control.setMode(mode_auto);
                updated = true;
                break;
            case climate::CLIMATE_MODE_FAN_ONLY:
               remote_control.setMode(mode_ventilate);
                updated = true;
                break;
            case climate::CLIMATE_MODE_OFF:
            default:
                remote_control.turnOFF();
                updated = true;
                break;
        }
    }

    if (call.get_target_temperature().has_value()){
        ESP_LOGV(
            "control", "Sending target temp: %.1f",
            *call.get_target_temperature()
        )
        remote_control.setTemperature(*call.get_target_temperature());
        this->target_temperature = *call.get_target_temperature();
        updated = true;
    }

    //const char* FAN_MAP[6]         = {"AUTO", "QUIET", "1", "2", "3", "4"};
    if (call.get_fan_mode().has_value()) {
        ESP_LOGV("control", "Requested fan mode is %s", *call.get_fan_mode());
        this->fan_mode = *call.get_fan_mode();
        switch(*call.get_fan_mode()) {
            case climate::CLIMATE_FAN_OFF:
                remote_contro.turnOFF();
                updated = true;
                break;
            case climate::CLIMATE_FAN_LOW:
                remote_contro.setSpeedFan(fan_speed_1);
                updated = true;
                break;
            case climate::CLIMATE_FAN_MEDIUM:
                remote_contro.setSpeedFan(fan_speed_2);
                updated = true;
                break;
            case climate::CLIMATE_FAN_MIDDLE:
                remote_contro.setSpeedFan(fan_speed_2);
                updated = true;
                break;
            case climate::CLIMATE_FAN_HIGH:
                remote_contro.setSpeedFan(fan_speed_3);
                updated = true;
                break;
            case climate::CLIMATE_FAN_ON:
            case climate::CLIMATE_FAN_AUTO:
            default:
                remote_contro.setSpeedFan(fan_auto);
                updated = true;
                break;
        }
    }

    //const char* VANE_MAP[7]        = {"AUTO", "1", "2", "3", "4", "5", "SWING"};
    if (call.get_swing_mode().has_value()) {
        ESP_LOGV(TAG, "DOING_NOTHING: control - requested swing mode is %s",
                *call.get_swing_mode());
    }

    ESP_LOGD(TAG, "control - Was HeatPump updated? %s", YESNO(updated));

    // send the update back to esphome:
    this->publish_state();
    // and the heat pump:
    remote_control.emit();
}

void Midea::setup() {
  pinMode(IR_EMITER, OUTPUT);
    // This will be called by App.setup()
    this->banner();
    this->dump_config();
}

void Midea::dump_config() {
    this->banner();
    ESP_LOGI(TAG, "  Supports HEAT: %s", YESNO(true));
    ESP_LOGI(TAG, "  Supports COOL: %s", YESNO(true));
}

void Midea::dump_state() {
    LOG_CLIMATE("", "Midea Climate", this);
    ESP_LOGI(TAG, "HELLO");
}
