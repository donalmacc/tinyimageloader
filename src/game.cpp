// Template, major revision 3
// IGAD/NHTV - Jacco Bikker - 2006-2009
// Abstracted away by Quinten Lansu (knight666) 
// (c) 2008 - 2010

#include "game.h"
#include "surface.h"
#include "Renderer.h"

#include "TinyImageLoader.h"

#include "stb_image_aug.h"

using namespace Tmpl8;
using namespace tb;
using namespace til;

Surface* show;
Surface* compare;

Image* loaded_32b;
uint32 frames;

Surface* show_16b;
Surface* show_32b;

Surface* animation[11];

bool flip = true;
bool allow_16bit = true;

//char image[] = "F:\\GameLab 3\\stable build\\Data\\Textures\\LightSuport_F2_Diffuse(512).png";
//char image[] = "media\\irrlichtlogoalpha2.tga";
//char image[] = "media\\Viper_Mark_II.bmp";
//char image[] = "media\\jump.gif";
//char image[] = "media\\img_test.png";
//char image[] = "media\\tooltileset.tga";
//char image[] = "media\\Tank.ico";
char image[] = "media\\Package.ico";
//char image[] = "media\\crossword.ico";
//char image[] = "BitComet.png";

static uint16 PixelConstruct(uint8 a_Red, uint8 a_Green, uint8 a_Blue)
{
	uint16 r = (a_Red << 8) & 0xF800;
	uint16 g = (a_Green << 4) & 0x07E0;
	uint16 b = (a_Blue) & 0x001F;

	return (r | g | b);
}

static uint16 PixelConvert_R8G8B8_R5G6B5(uint32 a_Color)
{
	uint16 r = (((a_Color & 0xFF0000) >> 16) << 8) & 0xF800;
	uint16 g = (((a_Color & 0x00FF00) >>  8) << 4) & 0x07E0;
	uint16 b = (((a_Color & 0x0000FF)      )     ) & 0x001F;

	return (r | g | b);
}


static uint32 Convert_R8G8B8_R5G6B5(uint32 a_Color)
{
	uint16 r = (((a_Color >> 16) >> 3) & 0x1F) << 10;
	uint16 g = (((a_Color >> 8 ) >> 3) & 0x1F) << 5 ;
	uint16 b = (((a_Color      ) >> 3) & 0x1F)      ;

	/*uint32 r = ((((a_Color & 0xF800) >> 8) << 8) / 0xF8) << 16;
	uint32 g = ((((a_Color & 0x07E0) >> 4) << 7) / 0x7E) << 8 ;
	uint32 b = ((((a_Color & 0x001F)     ) << 8) / 0x1F)      ;*/

	return (r | g | b);
}


static uint32 Convert_R5G6B5_R8G8B8(uint16 a_Color)
{
	/*uint32 r = ((a_Color >> 15) & 0x1 ) << 31;
	uint32 g = ((a_Color >> 10) & 0x1F) << 19;
	uint32 b = ((a_Color      ) & 0x1F) << 3 ;*/


	uint32 r = ((a_Color & 0xF800) >> 11);
	uint32 g = ((a_Color & 0x07E0) >> 5 );
	uint32 b = ((a_Color & 0x001F)      );

	r = ((r * 255) / 31) << 16;
	g = ((g * 255) / 63) << 8;
	b = (b * 255) / 31;

	/*uint32 r = ((((a_Color & 0xF800) >> 8) << 8) / 0xF8) << 16;
	uint32 g = ((((a_Color & 0x07E0) >> 4) << 7) / 0x7E) << 8 ;
	uint32 b = ((((a_Color & 0x001F)     ) << 8) / 0x1F)      ;*/

	return (r | g | b);
}


void Game::Init()
{
	TinyImageLoader::Init();

	Math::Seed();

	int width, height, channels;
	stbi_uc* data = stbi_load(image, &width, &height, &channels, 0);	

	til::uint32 option = TIL_FILE_ADDWORKINGDIR;
	if (allow_16bit)
	{
		til::Image* loaded_16b = TinyImageLoader::Load(image, option | TIL_DEPTH_R5G6B5);

		show = new Surface(loaded_16b->GetWidth(), loaded_16b->GetHeight());
		Pixel* buffer = show->GetBuffer();

		til::byte* src_buffer = loaded_16b->GetPixels();
		for (int i = 0; i < show->GetPitch() * show->GetHeight(); i++)
		{
			buffer[i] = Convert_R5G6B5_R8G8B8(*(uint16*)src_buffer);
			
			src_buffer += 2;
		}

		show_16b = new Surface(
			loaded_16b->GetWidth(), 
			loaded_16b->GetHeight(),	
			(Pixel*)buffer, 
			loaded_16b->GetWidth()
		);
	}
	
	loaded_32b = TinyImageLoader::Load(image, option | TIL_DEPTH_R8G8B8);

	frames = loaded_32b->GetFrameCount();
	for (uint32 i = 0; i < frames; i++)
	{
		til::uint32 width = loaded_32b->GetWidth(i);
		til::uint32 height = loaded_32b->GetHeight(i);
		animation[i] = new Surface(
			width, 
			height, 
			(Pixel*)loaded_32b->GetPixels(i),
			loaded_32b->GetWidth(i)
		);
	}

	show_32b = new Surface(
		loaded_32b->GetWidth(), 
		loaded_32b->GetHeight(),	
		(Pixel*)loaded_32b->GetPixels(), 
		loaded_32b->GetWidth()
	);



	//compare = new Surface(width, height, (Pixel*)data, width);

	//Image* test = new Image();
	//test->Load("testimage.png");
}

void Game::Tick(float a_DT)
{
	if (Keyboard::Released(QKEY_SPACE)) { flip = !flip; }
}

int current = 0;
float timestep = 0.f;
float delay = 5.f;

void Game::Render()
{
	m_Screen->Clear(0xFF0000);

	//show->CopyTo(m_Screen, 0, 0);

	animation[current]->CopyTo(m_Screen, SCRWIDTH / 2, 0);
	timestep += 0.01f;
	if (timestep > delay)
	{
		current = (++current > (frames - 1)) ? 0 : current;
		timestep -= delay;
	}

	if (allow_16bit)
	{
		if (flip) { show_32b->CopyTo(m_Screen, 0, 0); }
		else { show_16b->CopyTo(m_Screen, 0, 0); }
	}
	else
	{
		show_32b->CopyTo(m_Screen, 0, 0);
	}
	
}

void Game::Quit()
{
	Renderer::Quit();
}