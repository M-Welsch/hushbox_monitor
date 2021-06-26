#include "control_data.h"

void setup_thresholds(ControlData *control_data) {
    // Todo: take this data from eeprom or something ...
    control_data->overtemperature_threshold_0 = 60;
    control_data->overtemperature_threshold_1 = 60;
    control_data->lightsensor0_threshold = 1.7;
    control_data->lightsensor1_threshold = 1.7;
}

void add_table_row(char *buffer, String *html_buffer, const char *name, const char *unit, float data) {
    sprintf(buffer, "<tr><td>%s</td><td>%f %s</td></tr>", name, data, unit);
    html_buffer->concat(buffer);
}

void add_table_row(char *buffer, String *html_buffer, const char *name, const char *unit, int32 data) {
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
    add_table_row(buffer, &html_code, "Lightsensor 0 Voltage", "V", control_data->voltage_lightsensor0);
    add_table_row(buffer, &html_code, "Lightsensor 1 Voltage", "V", control_data->voltage_lightsensor1);
    add_table_row(buffer, &html_code, "Fan Voltage", "V", control_data->voltage_fans);
    add_table_row(buffer, &html_code, "Rdac", "digits", control_data->rdac);
    add_table_row(buffer, &html_code, "Overtemperature Threshold 0", "&#176;C", control_data->overtemperature_threshold_0);
    add_table_row(buffer, &html_code, "Overtemperature Threshold 1", "&#176;C", control_data->overtemperature_threshold_1);
    add_table_row(buffer, &html_code, "Lightsensor 0 Threshold", "V", control_data->lightsensor0_threshold);
    add_table_row(buffer, &html_code, "Lightsensor 1 Threshold", "V", control_data->lightsensor1_threshold);
    html_code.concat("</tbody></table>");
    return html_code;
}