#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#define NUM_PALETTES	256
#define IMAGE_WIDTH		512
#define IMAGE_HEIGHT	512
#define FACT			39
#define HALFFACT		19

class HelloWorld : public cocos2d::CCLayer
{
private:
	// 256 �ε��� �÷� ���� ����
	unsigned char *m_pPaletteBuffer;	// palette information (256 * RGBA)
	unsigned char *m_pIndexBuffer;		// index color image
	unsigned char *m_pImageBuffer;		// rgba raw image

	cocos2d::CCImage *m_pBitmap;		// �޸𸮻��� �̹��� ����ü
	cocos2d::CCSprite *m_pScreen;		// ��Ʈ���� �ؽ��ķ� ���� ��������Ʈ ��
	
	float	m_fElapsedTime;
	float   m_fScale;
	bool	m_bSpeedUp;

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void update(float dt);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	// virtual
	virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	//virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	int GetBuffer(int idx, int x, int y);
	void GetPaletteFromFile(char *filename, unsigned char *pPalette, int nNumColors);
};

#endif // __HELLOWORLD_SCENE_H__
