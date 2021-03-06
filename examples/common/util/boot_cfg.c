#include "util/boot_cfg.h"
#include "qapi/qapi_types.h"
#include "qapi/qapi.h"
#include "qapi/qapi_status.h"
#include "qapi/qapi_tlmm.h"
#include "qapi/qapi_timer.h"

#ifndef BOOT_CFG_PIN
#define BOOT_CFG_PIN 11
#endif

int boot_cfg(void) {
	qapi_GPIO_ID_t gpio_id;
	qapi_TLMM_Config_t tlmm_config;
	qapi_Status_t status = QAPI_OK;
	qapi_GPIO_Value_t val;
	
	memset(&tlmm_config, 0, sizeof(tlmm_config));
	tlmm_config.pin = BOOT_CFG_PIN; // Pin 11 is GPIO3
	tlmm_config.func = 0;
	tlmm_config.dir = QAPI_GPIO_INPUT_E;
	tlmm_config.pull = QAPI_GPIO_PULL_UP_E;
	tlmm_config.drive = QAPI_GPIO_2MA_E;
	status = qapi_TLMM_Get_Gpio_ID(&tlmm_config, &gpio_id);
	if(status != QAPI_OK) return 2;
	tlmm_config.pin = BOOT_CFG_PIN;

	status = qapi_TLMM_Config_Gpio(gpio_id, &tlmm_config);
	if(status != QAPI_OK) return 3;
	tlmm_config.pin = BOOT_CFG_PIN;

	// Wait some time to make sure the pin stabilized
	tx_thread_sleep(10);

	status = qapi_TLMM_Read_Gpio(gpio_id, BOOT_CFG_PIN, &val);
	if(status != QAPI_OK) return 4;

	status = qapi_TLMM_Release_Gpio_ID(&tlmm_config, gpio_id);
	if(status != QAPI_OK) return 5;

	if(val == QAPI_GPIO_LOW_VALUE_E) return 1;
	return 0;
}
