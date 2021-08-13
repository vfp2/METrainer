int BitRateFromSerial(std::string id)
{   
	int bitRate = 0;
	
    // find our bitrate
    if (id.find("QWR1") == 0)
        bitRate = 875000;
    if (id.find("QWR2") == 0)
        bitRate = 875000;
    if (id.find("QWP4") == 0)
        bitRate = 250000;
    if (id.find("PD")==0 && id.find("QK")==4)
    {
        bitRate = (id[6]-'0') * 100000;
        bitRate += (id[7]-'0') * 10000;
        bitRate += (id[8]-'0') * 1000;
    }
    if (id.find("PD")==0 && id.find("QM")==4)
    {
        bitRate = (id[6]-'0') * 1000000;
        bitRate += (id[7]-'0') * 100000;
        bitRate += (id[8]-'0') * 10000;
    }


    // bitrate if array
    int arraySize = 1;
    if (id.find("x") == 9)
    {
        arraySize = (id.at(10)-48);
        bitRate *= arraySize;
    }
	
	return bitRate;
}