//
// Created by Vetle Wegner Ingeberg on 23/03/2020.
//

#ifndef XIA_STRUCTURES_H
#define XIA_STRUCTURES_H


struct XIA_base {
    unsigned chanID : 4;
    unsigned slotID : 4;
    unsigned crateID : 4;
    unsigned headerLen : 5;
    unsigned eventLen : 14;
    bool finishCode : 1;
    unsigned event_ts_lo : 32;
    unsigned event_ts_hi : 16;
    unsigned cfd_result : 16;
    unsigned event_energy : 16;
    unsigned traceLen : 15;
    bool traceOutOfRange : 1;
};

struct XIA_esums {
    uint32_t energy_sum_trailing;
    uint32_t energy_sum_leading;
    uint32_t energy_sum_gap;
    uint32_t baseline;
};

struct XIA_qdc_sums {
    uint32_t qdc_sum[8];
};

struct XIA_ext_ts {
    uint32_t ext_ts_lo;
    uint32_t ext_ts_hi;
};

#endif // XIA_STRUCTURES_H
