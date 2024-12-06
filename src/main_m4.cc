#include "task_config.hh"
#include "logo.hh"

#include "libs/base/led.h"
#include "libs/base/gpio.h"

namespace coralmicro {
namespace {

    const char* PROJECT_NAME = "PCB Bringup";

    void setup_usr_button() {
        // Setup the usr button as an interrupt to allow for transition to SDP mode
        GpioConfigureInterrupt(
            Gpio::kUserButton, GpioInterruptMode::kIntModeFalling,
            [handle = xTaskGetCurrentTaskHandle()]() { xTaskResumeFromISR(handle); },
            /*debounce_interval_us=*/50 * 1e3);
    }

    void setup_tasks() {
        printf("Starting task creation...\r\n");
        fflush(stdout);
        
        // Create all tasks from configuration
        TaskErr_t ret = CreateM4Tasks();
        
        printf("Task creation returned: %d\r\n", static_cast<int>(ret));
        fflush(stdout);
        
        if (ret != TaskErr_t::OK) {
            printf("Failed to generate all tasks\r\n");
            fflush(stdout);
            // Suspend main task
            vTaskSuspend(nullptr);
        }
        else {
            printf("All tasks generated successfully\r\n");
            fflush(stdout);
        }
    }
    
    [[noreturn]] void Main() {
        printf("Starting main...\r\n");
        fflush(stdout);

        printf("\n%s\r\n", PROJECT_NAME);
        printf("Developed by JC \r\n");
        printf("%s\r\n", PROJECT_LOGO);
        fflush(stdout);

        LedSet(Led::kStatus, true);
        setup_usr_button();

        printf("About to create tasks...\r\n");
        fflush(stdout);
        
        setup_tasks();

        printf("Tasks created, starting main loop\r\n");
        fflush(stdout);

        while (true) {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
} // namespace
} // namespace coralmicro

extern "C" void app_main(void* param) {
    (void)param;
    
    printf("Pre-main initialization\r\n");
    
    coralmicro::Main();

    // Should never reach here
    vTaskSuspend(nullptr);
}