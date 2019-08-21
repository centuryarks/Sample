struct reg_value {
    u16 reg;
    u8 val;
};

static const struct reg_value imx378_setting_3840x2160_raw10[] = {
    {0x0136, 0x12},     // EXCK_FREQ[15:8]
    {0x0137, 0x00},     // EXCK_FREQ[7:0]
    /* Global Setting Header */
    {0xE000, 0x00},
    {0x4AE9, 0x18},
    {0x4AEA, 0x08},
    {0xF61C, 0x04},
    {0xF61E, 0x04},
    {0x4AE9, 0x21},
    {0x4AEA, 0x80},
    {0x38A8, 0x1F},
    {0x38A9, 0xFF},
    {0x38AA, 0x1F},
    {0x38AB, 0xFF},
    {0x55D4, 0x00},
    {0x55D5, 0x00},
    {0x55D6, 0x07},
    {0x55D7, 0xFF},
    {0x55E8, 0x07},
    {0x55E9, 0xFF},
    {0x55EA, 0x00},
    {0x55EB, 0x00},
    {0x574C, 0x07},
    {0x574D, 0xFF},
    {0x574E, 0x00},
    {0x574F, 0x00},
    {0x5754, 0x00},
    {0x5755, 0x00},
    {0x5756, 0x07},
    {0x5757, 0xFF},
    {0x5973, 0x04},
    {0x5974, 0x01},
    {0x5D13, 0xC3},
    {0x5D14, 0x58},
    {0x5D15, 0xA3},
    {0x5D16, 0x1D},
    {0x5D17, 0x65},
    {0x5D18, 0x8C},
    {0x5D1A, 0x06},
    {0x5D1B, 0xA9},
    {0x5D1C, 0x45},
    {0x5D1D, 0x3A},
    {0x5D1E, 0xAB},
    {0x5D1F, 0x15},
    {0x5D21, 0x0E},
    {0x5D22, 0x52},
    {0x5D23, 0xAA},
    {0x5D24, 0x7D},
    {0x5D25, 0x57},
    {0x5D26, 0xA8},
    {0x5D37, 0x5A},
    {0x5D38, 0x5A},
    {0x5D77, 0x7F},
    {0x7B75, 0x0E},
    {0x7B76, 0x0B},
    {0x7B77, 0x08},
    {0x7B78, 0x0A},
    {0x7B79, 0x47},
    {0x7B7C, 0x00},
    {0x7B7D, 0x00},
    {0x8D1F, 0x00},
    {0x8D27, 0x00},
    {0x9004, 0x03},
    {0x9200, 0x50},
    {0x9201, 0x6C},
    {0x9202, 0x71},
    {0x9203, 0x00},
    {0x9204, 0x71},
    {0x9205, 0x01},
    {0x9371, 0x6A},
    {0x9373, 0x6A},
    {0x9375, 0x64},
    {0x991A, 0x00},
    {0x996B, 0x8C},
    {0x996C, 0x64},
    {0x996D, 0x50},
    {0x9A4C, 0x0D},
    {0x9A4D, 0x0D},
    {0xA001, 0x0A},
    {0xA003, 0x0A},
    {0xA005, 0x0A},
    {0xA006, 0x01},
    {0xA007, 0xC0},
    {0xA009, 0xC0},
    /* Image Quality Adjustment Setting */
    {0x3D8A, 0x01},
    {0x4421, 0x08},
    {0x7B3B, 0x01},
    {0x7B4C, 0x00},
    {0x9905, 0x00},
    {0x9907, 0x00},
    {0x9909, 0x00},
    {0x990B, 0x00},
    {0x9944, 0x3C},
    {0x9947, 0x3C},
    {0x994A, 0x8C},
    {0x994B, 0x50},
    {0x994C, 0x1B},
    {0x994D, 0x8C},
    {0x994E, 0x50},
    {0x994F, 0x1B},
    {0x9950, 0x8C},
    {0x9951, 0x1B},
    {0x9952, 0x0A},
    {0x9953, 0x8C},
    {0x9954, 0x1B},
    {0x9955, 0x0A},
    {0x9A13, 0x04},
    {0x9A14, 0x04},
    {0x9A19, 0x00},
    {0x9A1C, 0x04},
    {0x9A1D, 0x04},
    {0x9A26, 0x05},
    {0x9A27, 0x05},
    {0x9A2C, 0x01},
    {0x9A2D, 0x03},
    {0x9A2F, 0x05},
    {0x9A30, 0x05},
    {0x9A41, 0x00},
    {0x9A46, 0x00},
    {0x9A47, 0x00},
    {0x9C17, 0x35},
    {0x9C1D, 0x31},
    {0x9C29, 0x50},
    {0x9C3B, 0x2F},
    {0x9C41, 0x6B},
    {0x9C47, 0x2D},
    {0x9C4D, 0x40},
    {0x9C6B, 0x00},
    {0x9C71, 0xC8},
    {0x9C73, 0x32},
    {0x9C75, 0x04},
    {0x9C7D, 0x2D},
    {0x9C83, 0x40},
    {0x9C94, 0x3F},
    {0x9C95, 0x3F},
    {0x9C96, 0x3F},
    {0x9C97, 0x00},
    {0x9C98, 0x00},
    {0x9C99, 0x00},
    {0x9C9A, 0x3F},
    {0x9C9B, 0x3F},
    {0x9C9C, 0x3F},
    {0x9CA0, 0x0F},
    {0x9CA1, 0x0F},
    {0x9CA2, 0x0F},
    {0x9CA3, 0x00},
    {0x9CA4, 0x00},
    {0x9CA5, 0x00},
    {0x9CA6, 0x1E},
    {0x9CA7, 0x1E},
    {0x9CA8, 0x1E},
    {0x9CA9, 0x00},
    {0x9CAA, 0x00},
    {0x9CAB, 0x00},
    {0x9CAC, 0x09},
    {0x9CAD, 0x09},
    {0x9CAE, 0x09},
    {0x9CBD, 0x50},
    {0x9CBF, 0x50},
    {0x9CC1, 0x50},
    {0x9CC3, 0x40},
    {0x9CC5, 0x40},
    {0x9CC7, 0x40},
    {0x9CC9, 0x0A},
    {0x9CCB, 0x0A},
    {0x9CCD, 0x0A},
    {0x9D17, 0x35},
    {0x9D1D, 0x31},
    {0x9D29, 0x50},
    {0x9D3B, 0x2F},
    {0x9D41, 0x6B},
    {0x9D47, 0x42},
    {0x9D4D, 0x5A},
    {0x9D6B, 0x00},
    {0x9D71, 0xC8},
    {0x9D73, 0x32},
    {0x9D75, 0x04},
    {0x9D7D, 0x42},
    {0x9D83, 0x5A},
    {0x9D94, 0x3F},
    {0x9D95, 0x3F},
    {0x9D96, 0x3F},
    {0x9D97, 0x00},
    {0x9D98, 0x00},
    {0x9D99, 0x00},
    {0x9D9A, 0x3F},
    {0x9D9B, 0x3F},
    {0x9D9C, 0x3F},
    {0x9D9D, 0x1F},
    {0x9D9E, 0x1F},
    {0x9D9F, 0x1F},
    {0x9DA0, 0x0F},
    {0x9DA1, 0x0F},
    {0x9DA2, 0x0F},
    {0x9DA3, 0x00},
    {0x9DA4, 0x00},
    {0x9DA5, 0x00},
    {0x9DA6, 0x1E},
    {0x9DA7, 0x1E},
    {0x9DA8, 0x1E},
    {0x9DA9, 0x00},
    {0x9DAA, 0x00},
    {0x9DAB, 0x00},
    {0x9DAC, 0x09},
    {0x9DAD, 0x09},
    {0x9DAE, 0x09},
    {0x9DC9, 0x0A},
    {0x9DCB, 0x0A},
    {0x9DCD, 0x0A},
    {0x9E17, 0x35},
    {0x9E1D, 0x31},
    {0x9E29, 0x50},
    {0x9E3B, 0x2F},
    {0x9E41, 0x6B},
    {0x9E47, 0x2D},
    {0x9E4D, 0x40},
    {0x9E6B, 0x00},
    {0x9E71, 0xC8},
    {0x9E73, 0x32},
    {0x9E75, 0x04},
    {0x9E94, 0x0F},
    {0x9E95, 0x0F},
    {0x9E96, 0x0F},
    {0x9E97, 0x00},
    {0x9E98, 0x00},
    {0x9E99, 0x00},
    {0x9EA0, 0x0F},
    {0x9EA1, 0x0F},
    {0x9EA2, 0x0F},
    {0x9EA3, 0x00},
    {0x9EA4, 0x00},
    {0x9EA5, 0x00},
    {0x9EA6, 0x3F},
    {0x9EA7, 0x3F},
    {0x9EA8, 0x3F},
    {0x9EA9, 0x00},
    {0x9EAA, 0x00},
    {0x9EAB, 0x00},
    {0x9EAC, 0x09},
    {0x9EAD, 0x09},
    {0x9EAE, 0x09},
    {0x9EC9, 0x0A},
    {0x9ECB, 0x0A},
    {0x9ECD, 0x0A},
    {0x9F17, 0x35},
    {0x9F1D, 0x31},
    {0x9F29, 0x50},
    {0x9F3B, 0x2F},
    {0x9F41, 0x6B},
    {0x9F47, 0x42},
    {0x9F4D, 0x5A},
    {0x9F6B, 0x00},
    {0x9F71, 0xC8},
    {0x9F73, 0x32},
    {0x9F75, 0x04},
    {0x9F94, 0x0F},
    {0x9F95, 0x0F},
    {0x9F96, 0x0F},
    {0x9F97, 0x00},
    {0x9F98, 0x00},
    {0x9F99, 0x00},
    {0x9F9A, 0x2F},
    {0x9F9B, 0x2F},
    {0x9F9C, 0x2F},
    {0x9F9D, 0x00},
    {0x9F9E, 0x00},
    {0x9F9F, 0x00},
    {0x9FA0, 0x0F},
    {0x9FA1, 0x0F},
    {0x9FA2, 0x0F},
    {0x9FA3, 0x00},
    {0x9FA4, 0x00},
    {0x9FA5, 0x00},
    {0x9FA6, 0x1E},
    {0x9FA7, 0x1E},
    {0x9FA8, 0x1E},
    {0x9FA9, 0x00},
    {0x9FAA, 0x00},
    {0x9FAB, 0x00},
    {0x9FAC, 0x09},
    {0x9FAD, 0x09},
    {0x9FAE, 0x09},
    {0x9FC9, 0x0A},
    {0x9FCB, 0x0A},
    {0x9FCD, 0x0A},
    {0xA14B, 0xFF},
    {0xA151, 0x0C},
    {0xA153, 0x50},
    {0xA155, 0x02},
    {0xA157, 0x00},
    {0xA1AD, 0xFF},
    {0xA1B3, 0x0C},
    {0xA1B5, 0x50},
    {0xA1B9, 0x00},
    {0xA24B, 0xFF},
    {0xA257, 0x00},
    {0xA2AD, 0xFF},
    {0xA2B9, 0x00},
    {0xB21F, 0x04},
    {0xB35C, 0x00},
    {0xB35E, 0x08},
    {0x0112, 0x08},
    {0x0113, 0x08},
    {0x0114, 0x01},
    {0x0342, 0x0D},
    {0x0343, 0xAC},
    {0x0340, 0x03},
    {0x0341, 0xE8},
    {0x0344, 0x03},
    {0x0345, 0xAC},
    {0x0346, 0x02},
    {0x0347, 0xC0},
    {0x0348, 0x0C},
    {0x0349, 0x2B},
    {0x034A, 0x09},
    {0x034B, 0x1F},
    {0x0220, 0x00},
    {0x0221, 0x11},
    {0x0381, 0x01},
    {0x0383, 0x01},
    {0x0385, 0x01},
    {0x0387, 0x01},
    {0x0900, 0x01},
    {0x0901, 0x22},
    {0x0902, 0x00},
    {0x3140, 0x02},
    {0x3C00, 0x00},
    {0x3C01, 0x01},
    {0x3C02, 0x9C},
    {0x3F0D, 0x00},
    {0x5748, 0x00},
    {0x5749, 0x00},
    {0x574A, 0x00},
    {0x574B, 0xA4},
    {0x7B53, 0x00},
    {0x9369, 0x73},
    {0x936B, 0x64},
    {0x936D, 0x5F},
    {0x9304, 0x03},
    {0x9305, 0x80},
    {0x9E9A, 0x2F},
    {0x9E9B, 0x2F},
    {0x9E9C, 0x2F},
    {0x9E9D, 0x00},
    {0x9E9E, 0x00},
    {0x9E9F, 0x00},
    {0xA2A9, 0x27},
    {0xA2B7, 0x03},
    {0x0401, 0x02},
    {0x0404, 0x00},
    {0x0405, 0x1B},
    {0x0408, 0x00},
    {0x0409, 0x02},
    {0x040A, 0x00},
    {0x040B, 0x02},
    {0x040C, 0x04},
    {0x040D, 0x3A},
    {0x040E, 0x03},
    {0x040F, 0x2C},
    {0x034C, 0x02},
    {0x034D, 0x80},
    {0x034E, 0x01},
    {0x034F, 0xE0},
    {0x0301, 0x05},
    {0x0303, 0x02},
    {0x0305, 0x03},
    {0x0306, 0x01},
    {0x0307, 0x5E},
    {0x0309, 0x08},
    {0x030B, 0x02},
    {0x030D, 0x03},
    {0x030E, 0x01},
    {0x030F, 0x40},
    {0x0310, 0x01},
    {0x0820, 0x07},
    {0x0821, 0x80},
    {0x0822, 0x00},
    {0x0823, 0x00},
    {0x3E20, 0x01},
    {0x3E37, 0x01},
    {0x3F50, 0x00},
    {0x3F56, 0x00},
    {0x3F57, 0x4B},
    /* MIPI setting */
    {0x0112, 0x0A},     // CSI_DT_FMT_H
    {0x0113, 0x0A},     // CSI_DT_FMT_L
    {0x0114, 0x03},     // CSI_LANE_MODE
    /* Frame Horizontal Clock Count */
    {0x0342, 0x11},     // LINE_LENGTH_PCK[15:8]
    {0x0343, 0xA0},     // LINE_LENGTH_PCK[7:0]
    /* Frame Vertical Clock Count */
    {0x0340, 0x0C},     // FRM_LENGTH_LINES[15:8]
    {0x0341, 0x1E},     // FRM_LENGTH_LINES[7:0]
    /* Visible Size */
    {0x0344, 0x00},     // X_ADD_STA[12:8]
    {0x0345, 0x6C},     // X_ADD_STA[7:0]
    {0x0346, 0x01},     // Y_ADD_STA[12:8]
    {0x0347, 0xB8},     // Y_ADD_STA[7:0]
    {0x0348, 0x0F},     // X_ADD_END[12:8]
    {0x0349, 0x6B},     // X_ADD_END[7:0]
    {0x034A, 0x0A},     // Y_ADD_END[12:8]
    {0x034B, 0x27},     // Y_ADD_END[7:0]
    /* Mode Setting */
    {0x0220, 0x00},     // HDR_MODE
    {0x0221, 0x11},     // HDR_RESO_REDU_V
    {0x0381, 0x01},     // X_EVN_INC
    {0x0383, 0x01},     // X_ODD_INC
    {0x0385, 0x01},     // Y_EVN_INC
    {0x0387, 0x01},     // Y_ODD_INC
    {0x0900, 0x00},     // BINNING_MODE
    {0x0901, 0x11},     // BINNING_TYPE_H
    {0x0902, 0x02},     // BINNING_WEIGHTING
    {0x3140, 0x02},
    {0x3C00, 0x00},
    {0x3C01, 0x03},
    {0x3C02, 0xDC},
    {0x3C03, 0x00},
    {0x3C04, 0x00},
    {0x3F0D, 0x00},
    {0x5748, 0x07},
    {0x5749, 0xFF},
    {0x574A, 0x00},
    {0x574B, 0x00},
    {0x7B53, 0x01},
    {0x9369, 0x5A},
    {0x936B, 0x55},
    {0x936D, 0x28},
    {0x9304, 0x03},
    {0x9305, 0x00},
    {0x9E9A, 0x2F},
    {0x9E9B, 0x2F},
    {0x9E9C, 0x2F},
    {0x9E9D, 0x00},
    {0x9E9E, 0x00},
    {0x9E9F, 0x00},
    {0xA2A9, 0x60},
    {0xA2B7, 0x00},
    /* Digital Crop & Scaling */
    {0x0401, 0x00},     // SCALE_MODE
    {0x0404, 0x00},     // SCALE_M[8]
    {0x0405, 0x10},     // SCALE_M[7:0]
    {0x0408, 0x00},     // DIG_CROP_X_OFFSET[12:8]
    {0x0409, 0x00},     // DIG_CROP_X_OFFSET[7:0]
    {0x040A, 0x00},     // DIG_CROP_Y_OFFSET[12:8]
    {0x040B, 0x00},     // DIG_CROP_Y_OFFSET[7:0]
    {0x040C, 0x0F},     // DIG_CROP_IMAGE_WIDTH[12:8]
    {0x040D, 0x00},     // DIG_CROP_IMAGE_WIDTH[7:0]
    {0x040E, 0x08},     // DIG_CROP_IMAGE_HEIGHT[12:8]
    {0x040F, 0x70},     // DIG_CROP_IMAGE_HEIGHT[7:0]
    /* Output Crop */
    {0x034C, 0x0F},     // X_OUT_SIZE[12:8]
    {0x034D, 0x00},     // X_OUT_SIZE[7:0]
    {0x034E, 0x08},     // Y_OUT_SIZE[12:8]
    {0x034F, 0x70},     // Y_OUT_SIZE[7:0]
    /* Clock Setting */
    {0x0301, 0x05},     // IVT_PXCK_DIV
    {0x0303, 0x02},     // IVT_SYCK_DIV
    {0x0305, 0x03},     // IVT_PREPLLCK_DIV
    {0x0306, 0x01},     // IVT_PLL_MPY[10:8]
    {0x0307, 0x5E},     // IVT_PLL_MPY[7:0]
    {0x0309, 0x0A},     // IOP_PXCK_DIV
    {0x030B, 0x01},     // IOP_SYCK_DIV
    {0x030D, 0x02},     // IOP_PREPLLCK_DIV
    {0x030E, 0x01},     // IOP_PLL_MPY[10:8]
    {0x030F, 0x5E},     // IOP_PLL_MPY[7:0]
    {0x0310, 0x00},     // PLL_MULT_DRIV
    {0x0820, 0x20},     // REQ_LINK_BIT_RATE_MBPS[31:24]
    {0x0821, 0xD0},     // REQ_LINK_BIT_RATE_MBPS[23:16]
    {0x0822, 0x00},     // REQ_LINK_BIT_RATE_MBPS[15:8]
    {0x0823, 0x00},     // REQ_LINK_BIT_RATE_MBPS[7:0]
    /* Output Data Select Setting */
    {0x3E20, 0x01},
    {0x3E37, 0x01},     // PDAF_CTRL1[7:0]
    /* PowerSave Setting */
    {0x3F50, 0x00},     // POWER_SAVE_ENABLE
    {0x3F56, 0x00},     // LINE_LENGTH_INCK[15:8]
    {0x3F57, 0x61},     // LINE_LENGTH_INCK[7:0]
    /* Coarse Storage Time (Exposure) */
    {0x0202, 0x03},     // COARSE_INTEG_TIME[15:8]
    {0x0203, 0xE8},     // COARSE_INTEG_TIME[7:0]
    /* Analog Gain */
    {0x0204, 0x00},     // ANA_GAIN_GLOBAL[9:8]
    {0x0205, 0x00},     // ANA_GAIN_GLOBAL[7:0]
    /* Digital Gain (White Balance) */
    {0x3FF9, 0x00},     // DPGA_USE_GLOBAL_GAIN
    {0x020E, 0x01},     // DIG_GAIN_GR[15:8]
    {0x020F, 0x00},     // DIG_GAIN_GR[7:0]
    {0x0210, 0x02},     // DIG_GAIN_R[15:8]
    {0x0211, 0x21},     // DIG_GAIN_R[7:0]
    {0x0212, 0x01},     // DIG_GAIN_B[15:8]
    {0x0213, 0xEE},     // DIG_GAIN_B[7:0]
    {0x0214, 0x01},     // DIG_GAIN_GB[15:8]
    {0x0215, 0x00},     // DIG_GAIN_GB[7:0]
    /* Pedestal (Black Level) */
    {0x3030, 0x01},     // MANUAL_DATA_PEDESTAL_EN
    {0x3032, 0x00},     // MANUAL_DATA_PEDESTAL_VALUE[11:8]
    {0x3033, 0x40},     // MANUAL_DATA_PEDESTAL_VALUE[7:0]
    /* Global Setting Footer */
    {0xF61C, 0x04},
    {0xF61E, 0x04},
    {0x4AE9, 0x18},
    {0x4AEA, 0x08},
    {0x4AE9, 0x21},
    {0x4AEA, 0x80},
};
