void GetPixelSize(int pixelNum, int *HPIXELS, int *VPIXELS)
{
	int i, w;
	int pixelTable[128];
	int pixelIdx = 0;
	double d, d1, d2, d3;
	double min = 0xFFFF;

	for (i = 128; i < 8192; i += 4)
	{
		if (pixelNum % i == 0)
		{
			pixelTable[pixelIdx] = i;
			pixelIdx++;
		}
	}

	for (i = 0; i < pixelIdx; i++)
	{
		w = pixelNum / pixelTable[i];
		d1 = fabs(w * 9.0 / pixelTable[i] - 16.0);	// 16:9
		d2 = fabs(w * 9.0 / pixelTable[i] - 12.0);	// 4:3
		d3 = fabs(w * 18.0 / pixelTable[i] - 12.0);	// HDR

		if (d1 < d2 && d1 < d3)
		{
			d = d1;
		}
		else if (d2 < d1 && d2 < d3)
		{
			d = d2;
		}
		else
		{
			d = d3;
		}

		if (min > d)
		{
			min = d;
			*HPIXELS = w;
			*VPIXELS = pixelTable[i];
		}
	}

	printf("RAW:%d x %d\n", *HPIXELS, *VPIXELS);
}
