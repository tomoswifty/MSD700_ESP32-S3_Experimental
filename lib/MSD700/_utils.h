
class utils
{
public:
    static float mapf(float x, float in_min, float in_max, float out_min, float out_max)
    {
        return static_cast<float>((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
    }
};