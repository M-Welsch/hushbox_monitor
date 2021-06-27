#include "control_data.h"

void setup_thresholds(ControlData *control_data) {
    // Todo: take this data from eeprom or something ...
    control_data->overtemperature_threshold_0 = 60;
    control_data->overtemperature_threshold_1 = 60;
    control_data->lightsensor0_threshold = 1.7;
    control_data->lightsensor1_threshold = 1.7;
    control_data->overtemperature_hysteresis_0 = 20;
    control_data->overtemperature_hysteresis_1 = 20;
    control_data->overtemperature_0 = false;
    control_data->overtemperature_1 = false;
    control_data->light_0_on = false;
    control_data->light_1_on = false;
}

void add_table_row(char *buffer, String *html_buffer, const char *name, const char *unit, float data) {
    sprintf(buffer, "<tr><td>%s</td><td>%.2f %s</td></tr>", name, data, unit);
    html_buffer->concat(buffer);
}

void add_table_row(char *buffer, String *html_buffer, const char *name, const char *unit, int32 data) {
    sprintf(buffer, "<tr><td>%s</td><td>%d %s</td></tr>", name, data, unit);
    html_buffer->concat(buffer);
}

void add_table_row(char *buffer, String *html_buffer, const char *name, const char *unit, bool data) {
    String state;
    if (data) {
        state = "true";
    } else {
        state = "false";
    }
    sprintf(buffer, "<tr><td>%s</td><td>%d %s</td></tr>", name, data, unit);
    html_buffer->concat(buffer);
}

String concat_data_table(ControlData *control_data) {
    char buffer[255];
    String html_code = "<table><thead><tr>";
    html_code.concat("<th>Parameter</th><th>Value</th>");
    html_code.concat("</tr></thead><tbody>");
    add_table_row(buffer, &html_code, "Ambient Temperature", "&#176;C", control_data->temperature_ambient);
    add_table_row(buffer, &html_code, "Probe 0 Temperature", "&#176;C", control_data->temperature_probe0);
    add_table_row(buffer, &html_code, "Probe 1 Temperature", "&#176;C", control_data->temperature_probe1);
    add_table_row(buffer, &html_code, "Overtemperature 0 ", "", control_data->overtemperature_0);
    add_table_row(buffer, &html_code, "Overtemperature 1 ", "", control_data->overtemperature_1);
    add_table_row(buffer, &html_code, "Light 0 On", "", control_data->light_0_on);
    add_table_row(buffer, &html_code, "Light 1 On", "", control_data->light_0_on);
    add_table_row(buffer, &html_code, "Lightsensor 0 Voltage", "V", control_data->voltage_lightsensor0);
    add_table_row(buffer, &html_code, "Lightsensor 1 Voltage", "V", control_data->voltage_lightsensor1);
    add_table_row(buffer, &html_code, "Fan Voltage", "V", control_data->voltage_fans);
    add_table_row(buffer, &html_code, "Rdac", "digits", control_data->rdac);
    add_table_row(buffer, &html_code, "Overtemperature Threshold 0", "&#176;C", control_data->overtemperature_threshold_0);
    add_table_row(buffer, &html_code, "Overtemperature Hysteresis 0", "&#176;C", control_data->overtemperature_hysteresis_0);
    add_table_row(buffer, &html_code, "Overtemperature Threshold 1", "&#176;C", control_data->overtemperature_threshold_1);
    add_table_row(buffer, &html_code, "Overtemperature Hysteresis 1", "&#176;C", control_data->overtemperature_hysteresis_1);
    add_table_row(buffer, &html_code, "Lightsensor 0 Threshold", "V", control_data->lightsensor0_threshold);
    add_table_row(buffer, &html_code, "Lightsensor 1 Threshold", "V", control_data->lightsensor1_threshold);
    html_code.concat("</tbody></table>");
    return html_code;
}

void check_for_fault_conditions(ControlData *d) {
    if (d->temperature_probe0 > d->overtemperature_threshold_0) {
        d->overtemperature_0 = true;
    } else {
        if (d->temperature_probe0 < (d->overtemperature_threshold_0 - d->overtemperature_hysteresis_0)) {
            d->overtemperature_0 = false;
        }
    }

    if (d->temperature_probe1 > d->overtemperature_threshold_1) {
        d->overtemperature_1 = true;
    } else {
        if (d->temperature_probe1 < (d->overtemperature_threshold_1 - d->overtemperature_hysteresis_1)) {
            d->overtemperature_1 = false;
        }
    }

    d->light_0_on = d->voltage_lightsensor0 < d->lightsensor0_threshold;
    d->light_1_on = d->voltage_lightsensor1 < d->lightsensor1_threshold;
}