enum {
    CC_ERR_NONE = 0,
    CC_ERR_EXPECT_DIGIT,
    CC_ERR_EXPECT_UNKNOW_TOKEN,
    CC_ERR_EXPECT_DIVISION_BY_ZERO,
    CC_ERR_EXPECT_REMAINDER_BY_ZERO,
};

int slm_eval(const char *str, int *err_type);
