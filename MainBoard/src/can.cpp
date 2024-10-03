#include <can.h>
#include <ACAN_T4.h>

void can_init() {
    ACAN_T4_Settings settings(125 * 1000);
    ACAN_T4::can1.begin(settings);
#ifdef DEBUG
    const unsigned int canError = ACAN_T4::can1.begin(settings);
    if (canError == 0x00) DEBUG_WRITE("Can 1 OK\n");
    else {
    DEBUG_WRITE("Can 1 initialization error: 0x"); DEBUG_WRITE(canError); DEBUG_WRITE("\n");
    }
    DEBUG_WRITE("Bitrate prescaler: ");
    DEBUG_WRITE(settings.mBitRatePrescaler);
    DEBUG_WRITE("\n");
    DEBUG_WRITE("Propagation Segment: ");
    DEBUG_WRITE(settings.mPropagationSegment);
    DEBUG_WRITE("\n");
    DEBUG_WRITE("Phase segment 1: ");
    DEBUG_WRITE(settings.mPhaseSegment1);
    DEBUG_WRITE("\n");
    DEBUG_WRITE("Phase segment 2: ");
    DEBUG_WRITE(settings.mPhaseSegment2);
    DEBUG_WRITE("\n");
    DEBUG_WRITE("RJW: ");
    DEBUG_WRITE(settings.mRJW);
    DEBUG_WRITE("Triple Sampling: ");
    DEBUG_WRITE(settings.mTripleSampling ? "yes" : "no");
    DEBUG_WRITE("\n");
    DEBUG_WRITE("Actual bitrate: ");
    DEBUG_WRITE(settings.actualBitRate ());
    DEBUG_WRITE(" bit/s");
    DEBUG_WRITE("\n");
    DEBUG_WRITE("Exact bitrate ? ");
    DEBUG_WRITE(settings.exactBitRate () ? "yes" : "no");
    DEBUG_WRITE("\n");
    DEBUG_WRITE("Distance from wished bitrate: ");
    DEBUG_WRITE(settings.ppmFromWishedBitRate ());
    DEBUG_WRITE(" ppm");
    DEBUG_WRITE("\n");
    DEBUG_WRITE("Sample point: ");
    DEBUG_WRITE(settings.samplePointFromBitStart ());
    DEBUG_WRITE("%");
    DEBUG_WRITE("\n");
#endif
}

unsigned long long can_rx() {
    CANMessage message;
    if (ACAN_T4::can1.receive(message)) {
        return message.data64;
    }
    struct Message _message;
}

bool can_tx(unsigned long long message_data) {
    CANMessage message;
    message.id = 1;
    message.data64 = message_data;
#ifdef DEBUG
    const bool ok = ACAN_T4::can1.tryToSend(message);
    if (ok) {
      gSentCount1 += 1 ;
    }
#endif
#ifndef DEBUG
    ACAN_T4::can1.tryToSend(message);
#endif
}
