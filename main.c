#include "usermodfx.h"

#define SAT_SOFTCLIP_C (0.3f)

static float amp, alg;

void MODFX_INIT(uint32_t platform, uint32_t api) {
  (void)platform; (void)api;
}

void MODFX_PROCESS(const float *main_xn, float *main_yn,
                   const float *sub_xn, float *sub_yn,
                   uint32_t frames) {
  (void)*sub_xn; (void)*sub_yn;
  
  for (uint32_t i = 0; i < frames; i++) {
    const float in_l   = main_xn[i * 2];
    const float in_r   = main_xn[i * 2 + 1];

    const float in_m   = (in_l + in_r) * (amp * 3.0f + 0.5f);
    const float softcl = (fx_softclipf(SAT_SOFTCLIP_C, in_m)
			  * (1.0f / (1.0f - SAT_SOFTCLIP_C)));
    const float schetz = fx_sat_schetzenf(in_m);
    const float out_m  = ((1.0f - alg) * softcl
			  + alg        * schetz);
    
    main_yn[i * 2]     = out_m;
    main_yn[i * 2 + 1] = out_m;
  }
}

void MODFX_PARAM(uint8_t idx, int32_t val) {
  const float val_f = q31_to_f32(val);

  switch (idx) {
  case k_user_modfx_param_time:
    alg = val_f;
    break;
  case k_user_modfx_param_depth:
    amp = val_f;
    break;
  default:
    break;
  }
}
