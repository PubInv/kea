#include "kea_array.h"
#include "kea_debug.h"
#include "kea_task.h"
#include "kea_core.h"

class PanelBlinkTask : public Kea::Task
{
public:
    unsigned long half_period_ms = 500;
    unsigned long time_of_last_change_ms = 0;

private:
    bool _init() override;
    bool _run() override;
};

bool PanelBlinkTask::_init()
{
    return true;
}

bool PanelBlinkTask::_run()
{
    // unsigned long ms = millis();
    // HAL *hal = (HAL *)getConfig()->hal;
    // if (hal->panel->status == LED_STATUS::BLINKING)
    // {
    //     if (ms - time_of_last_change_ms > half_period_ms)
    //     {
    //         hal->panel->setStatusLED(!hal->panel->isStatusLEDOnOrOff());
    //         time_of_last_change_ms = ms;
    //     }
    // }
    return true;
}

namespace Kea
{
class MachineConfig {
public:
  MachineConfig();
};
} // namespace Kea

int main(void)
{
    Kea::Core core;
    Kea::MachineConfig machineConfig;

    PanelBlinkTask panelBlinkTask;

    Kea::TaskProperties PanelBlinkProperties;
    PanelBlinkProperties.name = "PanelBlink";
    PanelBlinkProperties.id = 30;
    PanelBlinkProperties.period = 500; // MachineConfig::INIT_PANEL_BLINK_PERIOD_MS;
    PanelBlinkProperties.priority = Kea::TaskPriority::High;
    PanelBlinkProperties.state_and_config = (void *)&machineConfig;
    bool panelBlinkAdd = core.AddTask(&panelBlinkTask, &PanelBlinkProperties);

    if (!panelBlinkAdd)
    {
        Kea::Debug<const char *>("Failed to add task\n");
    }

    return 0;
}
