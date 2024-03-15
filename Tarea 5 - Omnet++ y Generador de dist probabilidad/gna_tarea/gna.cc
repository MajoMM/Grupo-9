#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class nodo : public cSimpleModule {
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

private:
    cMessage *delayEvent = nullptr;
    simtime_t lastArrivalTime;
    cHistogram histograma_tiempo_entre_llegadas;

    // Parámetros del generador lineal congruencial (GLC)
    unsigned long a = 16807;
    unsigned long c = 0;
    unsigned long m = 2147483647; // 2^31 - 1
    unsigned long seed = 123456789; // Semilla inicial

public:
    virtual ~nodo();
    double lcgRandom();
};

Define_Module(nodo);

nodo::~nodo() {
    cancelAndDelete(delayEvent);
}

void nodo::initialize() {
    // Creando el mensaje delayEvent
    delayEvent = new cMessage("Intervalo entre envios");
    lastArrivalTime = -1;
    histograma_tiempo_entre_llegadas.setName("Tiempo entre llegadas");

    // Enviar mensaje (nodo tx)
    if (strcmp("tx", getName()) == 0) {
        EV << "Enviando mensaje inicial...\n";
        cMessage *msg = new cMessage("tictocMsg");
        send(msg, "salida");

        // Generando el retardo inicial con el generador lineal congruencial
        double tiempo = lcgRandom();
        EV << "Generando el retardo inicial: " << tiempo << "s\n";
        scheduleAt(simTime() + tiempo, delayEvent);
    }
}

void nodo::handleMessage(cMessage *msg) {
    if (msg == delayEvent) {
        if (strcmp("tx", getName()) == 0) {
            EV << "Enviando mensaje nuevamente...\n";
            cMessage *msg = new cMessage("tictocMsg");
            send(msg, "salida");

            // Generando el retardo nuevamente con el generador lineal congruencial
            double tiempo = lcgRandom();
            EV << "Generando el retardo nuevamente: " << tiempo << "s\n";
            scheduleAt(simTime() + tiempo, delayEvent);
        }
    } else {
        simtime_t currentArrivalTime = simTime();
        EV << "Mensaje recibido...\n";

        if (lastArrivalTime >= 0) {
            simtime_t interArrivalTime = currentArrivalTime - lastArrivalTime;
            EV << "Inter-arrival time: " << interArrivalTime << "s\n";
            recordScalar("interArrivalTime", interArrivalTime);
            histograma_tiempo_entre_llegadas.collect(interArrivalTime);
        }

        lastArrivalTime = currentArrivalTime;
        delete msg;
    }
}

double nodo::lcgRandom() {
    seed = (a * seed + c) % m;
    return static_cast<double>(seed) / m;
}

void nodo::finish() {
    histograma_tiempo_entre_llegadas.recordAs("Inter-Arrival Time Histogram");
}
