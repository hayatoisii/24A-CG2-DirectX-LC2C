struct PixeelShanderOutput {
float32_t4 color : SV_TARGET0;
};

PixelShanderOutput main() {
    PixelShanderOutput output;
    output.clor = float32_t4(1.0, 1.0, 1.0,1.0);
    return output;
}