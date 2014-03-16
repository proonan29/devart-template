#include "HelloWorldScene.h"

typedef   struct
{
	char header[16];
	unsigned char num_cols[2];
}	ColHeader;

ColHeader head;

unsigned  char palette[NUM_PALETTES*3];

USING_NS_CC;

// 팔레트 생성, RGB -> RGBA값으로 전환
void BuildPal(unsigned char *rgb, unsigned char *rgba)
{
	int i, j;

	i = 0;
	for (j=0; j<NUM_PALETTES*3; j+=3)
	{
		rgba[i++] = rgb[j]<<2;
		rgba[i++] = rgb[j+1]<<2;
		rgba[i++] = rgb[j+2]<<2;
		rgba[i++] = 255;
	}
}

void IndexToImage(unsigned char *pIndexBuffer, unsigned char *pImageBuffer, unsigned char *pPalette, int nSize)
{
	int *pPalBuffer = (int *)pPalette;
	int *pImgBuffer = (int *)pImageBuffer;

	for (int i=0; i<nSize; i++)
	{
		pImgBuffer[i] = pPalBuffer[pIndexBuffer[i]];
	}
}

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

// 주어진 파일로부터 RGBA를 가져온다.
void HelloWorld::GetPaletteFromFile(char *filename, unsigned char *pPalette, int nNumColors)
{
	CCImage *pImage = new CCImage();
	CCTexture2D *pTex = new CCTexture2D();

	pImage->initWithImageFile(filename, CCImage::kFmtPng);

	memcpy(pPalette, pImage->getData(), nNumColors * 4); // RGBA = 4 bytes
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	m_fElapsedTime = 0;
	m_fScale = 1.0;
	m_bSpeedUp = false;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Map Maker", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);


	// 버퍼 생성 및 팔레트 로딩
	m_pPaletteBuffer = new unsigned char[NUM_PALETTES * 4];				// RGBA * 256 인덱스 컬러
	m_pIndexBuffer	 = new unsigned char[IMAGE_WIDTH * IMAGE_HEIGHT];	// 인덱스 컬러로 된 이미지 버퍼
	m_pImageBuffer   = new unsigned char[IMAGE_WIDTH * IMAGE_HEIGHT*4];	// RGBA * 512 * 512

	/*
	// LAND.COL로 부터 팔레트 로딩
	std::string sFilePath = std::string("");
	sFilePath += CCFileUtils::sharedFileUtils()->fullPathForFilename("LAND.COL");

	FILE *pal_file = fopen(sFilePath.c_str(), "rb");
	if (pal_file)
    {
		fread(&head, sizeof(ColHeader), 1, pal_file);
		//if (head.num_cols > NUM_PALETTES)
		//	head.num_cols = NUM_PALETTES;
		fread(palette, 256*3, 1, pal_file);
		fclose(pal_file);

		// 팔레트 변환 RGB -> RGBA
		BuildPal(palette, m_pPaletteBuffer);
    }
	*/

	// PNG로부터 팔레트 로딩
	GetPaletteFromFile("mapheight.png", m_pPaletteBuffer, 256);

    // add generating "Map" screen"

	// 인덱스 컬러로 된 이미지에 랜덤 값을 채운다.
	for (int i = 0; i < IMAGE_WIDTH*IMAGE_HEIGHT; i++)
		m_pIndexBuffer[i] = rand()%NUM_PALETTES;
	// 인덱스 -> 이미지 변환
	IndexToImage(m_pIndexBuffer, m_pImageBuffer, m_pPaletteBuffer, IMAGE_WIDTH*IMAGE_HEIGHT);

	m_pBitmap = new CCImage();
	m_pBitmap->initWithImageData(m_pImageBuffer, IMAGE_WIDTH*IMAGE_HEIGHT*4, CCImage::kFmtRawData, IMAGE_WIDTH, IMAGE_HEIGHT, 8);
	CCTexture2D *pTex = new CCTexture2D();
	pTex->initWithImage(m_pBitmap);
    m_pScreen = CCSprite::createWithTexture(pTex);
	pTex->release();
	m_pBitmap->release();

    // position the sprite on the center of the screen
    m_pScreen->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	m_pScreen->setScale( 2.0 );

    // add the sprite as a child to this layer
    this->addChild(m_pScreen, 0);

	setTouchEnabled(true);
	this->scheduleUpdate();
    
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void HelloWorld::update(float dt)
{
	bool bComputeAgain = false;

	if (m_bSpeedUp)
		m_fElapsedTime = 1.0;
	else
		m_fElapsedTime += dt * 0.5;
	//m_fScale = pow(2.0f, m_fElapsedTime);

	if (m_fElapsedTime >= 1.0)
	//if (m_fScale >= 2.0)
	{
		//m_fScale /= 2.0;
		//m_fElapsedTime = 0;
		m_fElapsedTime = fmod(m_fElapsedTime, 1.0f);
		bComputeAgain = true;
	}

	//static int off = 0;

	// RGBA raw data 세팅함.
	/*
	float c1,c2,c3,c4,c5,c6,v;
	int indexColor;
	int offset = 0;

    c1=(float)(rand()%5+1);
    c2=(float)(rand()%5+1);
    c3=(float)(rand()%5+1);
    c4=(float)(rand()%10+1);
    c5=(float)(rand()%512+1);
    c6=(float)(rand()%512+1);

	for (int i=0; i<IMAGE_HEIGHT; i++)
	{
		for (int j=0; j<IMAGE_WIDTH; j++)
	    {
			v = cos((float)(j-256+c5)/(cos((float)i/c1)+c3))+
			  cos((float)(j)*(float)(i/c2)+c6)+
			  cos((float)(j/c3)*(float)(i/c4)+c5);

			// 최종 결과물 = 높이값
			indexColor = v*10.5+64;
			if (indexColor<0)
				indexColor = 0;
			else if (indexColor>255)
				indexColor=255; //(indexColor-64)*8;

			m_pIndexm_pIndexBuffer[offset++] = indexColor;
		}
	}
	*/
	int i,   j;
	int x_q1, x_q3;
	int y_q1, y_q3;

	if (bComputeAgain)
	{
		// 각각 25%, 75% 지점의 좌표를 구한다.
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

		// 가운데 4 pixel에 랜덤값을 더함.
		m_pIndexBuffer[(IMAGE_HEIGHT/2-1)*IMAGE_WIDTH+(IMAGE_WIDTH/2-1)]+=rand()%6-2;
		m_pIndexBuffer[(IMAGE_HEIGHT/2-1)*IMAGE_WIDTH+(IMAGE_WIDTH/2)]+=rand()%5-2;
		m_pIndexBuffer[(IMAGE_HEIGHT/2)*IMAGE_WIDTH+(IMAGE_WIDTH/2-1)]+=rand()%5-2;
		m_pIndexBuffer[(IMAGE_HEIGHT/2)*IMAGE_WIDTH+(IMAGE_WIDTH/2)]+=rand()%5-2;

		// 인덱스 -> 이미지 변환
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

	/*
    // position the sprite on the center of the screen
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    m_pScreen->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(m_pScreen, 0);
	*/

	// 스프라이트의 크기를 조정
	m_pScreen->setScale(1.0+m_fElapsedTime);
	//m_pScreen->setScale(1.0+m_fScale);
}

// 버튼 양쪽을 동시에 체크 가능하도록 설정
void HelloWorld::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
	m_bSpeedUp = true;
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	m_bSpeedUp = false;
}