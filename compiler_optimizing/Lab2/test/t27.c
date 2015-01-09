int func ()
{
	int i = 90;

	//goto L1;
	while (i > 4 )
	{
		 i --;
		if (i < 10)
		{
			//printf("i = %d \n",3*i);
			i = i-2;
			continue;
		}

		else if (i < 56)
		{
			//printf("i = %d \n",2*i);
			i = i-3;
			continue;
		}
		//else 
			//printf("i = %d \n",i);
		
	}
}