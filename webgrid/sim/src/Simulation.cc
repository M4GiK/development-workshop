#include "Simulation.hh"

Simulation::Simulation(Simulation* rOOt): EventHandler(), EventListener(rOOt) {
    enableTrace = false;
    if (rOOt == NULL) {
        setHandler(this);
    }
}

Simulation::~Simulation() {
}

void Simulation::run() {
    EventNode* event;

    init();
    while (!isFinish()) {
        event = nextEvent();
        if (!event) error("Simulation.cc: No more events");
        event->dest->process(event);
        updateStat(event);
        delete event;
    }
    finalize();
    report();
}
