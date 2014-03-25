# Infinity Map Maker

## Authors
[Donghee (Gini) Hong](https://github.com/proonan29)

## Description
Making random map on any device

As a gamemaker, I was wondering how to create beautiful scenary from just random numbers. The method I found out can generate unlimited 2D maps of ocean, sea, mountains, lakes, and natural seashore. Bring out any smart phone or tablet, it can give hours of fun to watch. Rendered image can be used as map for other games.

## Link to Prototype
[https://github.com/proonan29/devart-template/tree/master/project_code](https://github.com/proonan29/devart-template/tree/master/project_code)

## Example Code
```
int HelloWorld::GetBuffer(int idx, int x, int y)
{
   int sum, idx1;

   sum=0;
   if (idx>=IMAGE_WIDTH)
      sum+=m_pIndexBuffer[idx-IMAGE_WIDTH];
   if (idx<IMAGE_WIDTH*(IMAGE_HEIGHT-1))
      sum+=m_pIndexBuffer[idx+IMAGE_WIDTH];
   if ((idx%IMAGE_WIDTH)>0)
      sum+=m_pIndexBuffer[idx-1];
   if ((idx%IMAGE_WIDTH)<(IMAGE_WIDTH-1))
      sum+=m_pIndexBuffer[idx+1];

   if ((x!=0)&&(y!=0))
   {
      idx1=idx + x + y*IMAGE_WIDTH;
      sum+=m_pIndexBuffer[idx1]*3;
      sum/=7;
      sum=m_pIndexBuffer[idx]+(sum-m_pIndexBuffer[idx])/4;
   }
   else
   {
      sum/=4;
      sum=m_pIndexBuffer[idx]+(sum-m_pIndexBuffer[idx])/2;
   }

   return(sum+rand()%FACT-HALFFACT);
}

void update(float dt)
{
	int i,   j;
	int x_q1, x_q3;
	int y_q1, y_q3;

	if (bComputeAgain)
	{
		// position of 25%, 75% location of width & height
		x_q1 = IMAGE_WIDTH/4;
		x_q3 = IMAGE_WIDTH - IMAGE_WIDTH/4 -1;
		y_q1 = IMAGE_HEIGHT/4;
		y_q3 = IMAGE_HEIGHT - IMAGE_HEIGHT/4 -1;


		for (i=0; i<IMAGE_HEIGHT/4; i++)
		{
			for (j=0; j<IMAGE_WIDTH/4; j++)
			{
				m_pIndexBuffer[i*2*IMAGE_WIDTH+j*2]  = GetBuffer((y_q1+i)*IMAGE_WIDTH+x_q1+j,-1,-1);
				m_pIndexBuffer[i*2*IMAGE_WIDTH+j*2+1]= GetBuffer((y_q1+i)*IMAGE_WIDTH+x_q1+j,0,-1);
				m_pIndexBuffer[i*2*IMAGE_WIDTH+IMAGE_WIDTH-j*2-1]=GetBuffer((y_q1+i)*IMAGE_WIDTH+x_q3-j,1,-1);
				m_pIndexBuffer[i*2*IMAGE_WIDTH+IMAGE_WIDTH-j*2-2]=GetBuffer((y_q1+i)*IMAGE_WIDTH+x_q3-j,0,-1);

				m_pIndexBuffer[(i*2+1)*IMAGE_WIDTH+j*2]=GetBuffer((y_q1+i)*IMAGE_WIDTH+x_q1+j,-1,0);
				m_pIndexBuffer[(i*2+1)*IMAGE_WIDTH+j*2+1]=GetBuffer((y_q1+i)*IMAGE_WIDTH+x_q1+j,0,0);
				m_pIndexBuffer[(i*2+2)*IMAGE_WIDTH-j*2-1]=GetBuffer((y_q1+i)*IMAGE_WIDTH+x_q3-j,1,0);
				m_pIndexBuffer[(i*2+2)*IMAGE_WIDTH-j*2-2]=GetBuffer((y_q1+i)*IMAGE_WIDTH+x_q3-j,0,0);

				m_pIndexBuffer[(IMAGE_HEIGHT-1-i*2)*IMAGE_WIDTH+j*2]=GetBuffer((y_q3-i)*IMAGE_WIDTH+x_q1+j,-1,1);
				m_pIndexBuffer[(IMAGE_HEIGHT-1-i*2)*IMAGE_WIDTH+j*2+1]=GetBuffer((y_q3-i)*IMAGE_WIDTH+x_q1+j,0,1);
				m_pIndexBuffer[(IMAGE_HEIGHT-1-i*2+1)*IMAGE_WIDTH-j*2-1]=GetBuffer((y_q3-i)*IMAGE_WIDTH+x_q3-j,1,1);
				m_pIndexBuffer[(IMAGE_HEIGHT-1-i*2+1)*IMAGE_WIDTH-j*2-2]=GetBuffer((y_q3-i)*IMAGE_WIDTH+x_q3-j,0,1);

				m_pIndexBuffer[(IMAGE_HEIGHT-2-i*2)*IMAGE_WIDTH+j*2]=GetBuffer((y_q3-i)*IMAGE_WIDTH+x_q1+j,-1,0);
				m_pIndexBuffer[(IMAGE_HEIGHT-2-i*2)*IMAGE_WIDTH+j*2+1]=GetBuffer((y_q3-i)*IMAGE_WIDTH+x_q1+j,0,0);
				m_pIndexBuffer[(IMAGE_HEIGHT-2-i*2+1)*IMAGE_WIDTH-j*2-1]=GetBuffer((y_q3-i)*IMAGE_WIDTH+x_q3-j,1,0);
				m_pIndexBuffer[(IMAGE_HEIGHT-2-i*2+1)*IMAGE_WIDTH-j*2-2]=GetBuffer((y_q3-i)*IMAGE_WIDTH+x_q3-j,0,0);
			}
		}

		// addrandom numbers to center 4 pixels
		m_pIndexBuffer[(IMAGE_HEIGHT/2-1)*IMAGE_WIDTH+(IMAGE_WIDTH/2-1)]+=rand()%6-2;
		m_pIndexBuffer[(IMAGE_HEIGHT/2-1)*IMAGE_WIDTH+(IMAGE_WIDTH/2)]+=rand()%5-2;
		m_pIndexBuffer[(IMAGE_HEIGHT/2)*IMAGE_WIDTH+(IMAGE_WIDTH/2-1)]+=rand()%5-2;
		m_pIndexBuffer[(IMAGE_HEIGHT/2)*IMAGE_WIDTH+(IMAGE_WIDTH/2)]+=rand()%5-2;

		// index toimage
		IndexToImage(m_pIndexBuffer, m_pImageBuffer, m_pPaletteBuffer, IMAGE_WIDTH*IMAGE_HEIGHT);
		//off++;

		m_pBitmap = new CCImage();
		m_pBitmap->initWithImageData(m_pImageBuffer, IMAGE_WIDTH*IMAGE_HEIGHT*4, CCImage::kFmtRawData, IMAGE_WIDTH, IMAGE_HEIGHT, 8);
		CCTexture2D *pTex = new CCTexture2D();
		pTex->initWithImage(m_pBitmap);
		//m_pScreen->removeFromParentAndCleanup(true);
		//m_pScreen = CCSprite::createWithTexture(pTex);
		m_pScreen->setTextureRect(CCRectMake(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT));
		m_pScreen->setTexture(pTex);
		pTex->release();
		m_pBitmap->release();
	}
}
```
## Links to External Libraries
[Cocos2d-x](http://www.cocos2d-x.org)

## Images & Videos
![Cover Image](project_images/mapmaker.png?raw=true "Cover Image")

https://www.youtube.com/watch?v=DGrZAm3a6V0
