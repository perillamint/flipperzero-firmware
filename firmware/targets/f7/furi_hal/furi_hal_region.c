#include <furi_hal_region.h>
#include <furi_hal_version.h>

const FuriHalRegion furi_hal_region_zero = {
    .country_code = "00",
    .bands_count = 1,
    .bands = {
        {
            .start = 0,
            .end = 1000000000,
            .power_limit = 12,
            .duty_cycle = 50,
        },
    }};

const FuriHalRegion furi_hal_region_eu_ru = {
    .country_code = "EU",
    .bands_count = 2,
    .bands = {
        {
            .start = 433050000,
            .end = 434790000,
            .power_limit = 12,
            .duty_cycle = 50,
        },
        {
            .start = 868150000,
            .end = 868550000,
            .power_limit = 12,
            .duty_cycle = 50,
        }}};

const FuriHalRegion furi_hal_region_us_ca_au = {
    .country_code = "US",
    .bands_count = 3,
    .bands = {
        {
            .start = 304100000,
            .end = 321950000,
            .power_limit = 12,
            .duty_cycle = 50,
        },
        {
            .start = 433050000,
            .end = 434790000,
            .power_limit = 12,
            .duty_cycle = 50,
        },
        {
            .start = 915000000,
            .end = 928000000,
            .power_limit = 12,
            .duty_cycle = 50,
        }}};

const FuriHalRegion furi_hal_region_jp = {
    .country_code = "JP",
    .bands_count = 2,
    .bands = {
        {
            .start = 312000000,
            .end = 315250000,
            .power_limit = 12,
            .duty_cycle = 50,
        },
        {
            .start = 920500000,
            .end = 923500000,
            .power_limit = 12,
            .duty_cycle = 50,
        }}};

const FuriHalRegion furi_hal_region_kr = {
    .country_code = "KR",
    .bands_count = 2,
    .bands = {
        //{
        //    .start = 235000000, // 시각장애인 유도신호등, 해당 기능 사용시만 주석 해제하시오`
        //    .end = 267000000,
        //    .power_limit = 12,
        //    .duty_cycle = 50,
        //},
        //{
        //    .start = 335400000, // 시각장애인 유도신호등, 해당 기능 사용시만 주석 해제하시오`
        //    .end = 368500000,
        //    .power_limit = 12,
        //    .duty_cycle = 50,
        //},
        {
            .start = 420000000, // 간이무선국, 레저무전기, 차키, 아마추어, 콜택시 무전기, 운송업무
            .end = 460000000,
            .power_limit = 12,
            .duty_cycle = 50,
        },
        //{
        //    .start = 460000000, // 건물 무전기
        //    .end = 470000000,
        //    .power_limit = 12,
        //    .duty_cycle = 50,
        //},
        //{
        //    .start = 470000000, // 방송 무선마이크
        //    .end = 698000000,
        //    .power_limit = 12,
        //    .duty_cycle = 50,
        //},
        {
            .start = 894000000, // 삐삐, BIS, 무선마이크(비인가)
            .end = 942000000,
            .power_limit = 12,
            .duty_cycle = 50,
        }}};

static const FuriHalRegion* furi_hal_region = NULL;

void furi_hal_region_init() {
    FuriHalVersionRegion region = furi_hal_version_get_hw_region();

    // For now, make it Korean only
    furi_hal_region = &furi_hal_region_kr;
    return;

    if(region == FuriHalVersionRegionUnknown) {
        furi_hal_region = &furi_hal_region_zero;
    } else if(region == FuriHalVersionRegionEuRu) {
        furi_hal_region = &furi_hal_region_eu_ru;
    } else if(region == FuriHalVersionRegionUsCaAu) {
        furi_hal_region = &furi_hal_region_us_ca_au;
    } else if(region == FuriHalVersionRegionJp) {
        furi_hal_region = &furi_hal_region_jp;
    }
}

const FuriHalRegion* furi_hal_region_get() {
    return furi_hal_region;
}

void furi_hal_region_set(FuriHalRegion* region) {
    furi_hal_region = region;
}

bool furi_hal_region_is_provisioned() {
    return furi_hal_region != NULL;
}

const char* furi_hal_region_get_name() {
    if(furi_hal_region) {
        return furi_hal_region->country_code;
    } else {
        return "--";
    }
}

bool furi_hal_region_is_frequency_allowed(uint32_t frequency) {
    if(!furi_hal_region) {
        return false;
    }

    const FuriHalRegionBand* band = furi_hal_region_get_band(frequency);
    if(!band) {
        return false;
    }

    return true;
}

const FuriHalRegionBand* furi_hal_region_get_band(uint32_t frequency) {
    if(!furi_hal_region) {
        return NULL;
    }

    for(size_t i = 0; i < furi_hal_region->bands_count; i++) {
        if(furi_hal_region->bands[i].start <= frequency &&
           furi_hal_region->bands[i].end >= frequency) {
            return &furi_hal_region->bands[i];
        }
    }

    return NULL;
}
