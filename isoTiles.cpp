#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class IsometricDemo : public olc::PixelGameEngine {
public:
	IsometricDemo() {
		sAppName = "Isometric Tiles";
	}

private:
	olc::vi2d vWorldSize{ 14, 10 };
	olc::vi2d vTileSize{ 40, 20 };
	olc::vi2d vOrigin{ 5, 1 };

	olc::Sprite *sprIsom = nullptr;
	int *pWorld = nullptr;

public:
	bool OnUserCreate() override {
		sprIsom = new olc::Sprite("isometric_demo.png");
		pWorld = new int[vWorldSize.x * vWorldSize.y]{ 0 };
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::WHITE);

		auto ToScreen = [&](int x, int y) {
			return olc::vi2d{
				(vOrigin.x * vTileSize.x) + (x - y) * (vTileSize.x / 2),
				(vOrigin.y * vTileSize.y) + (x + y) * (vTileSize.y / 2)
			};
		};

		SetPixelMode(olc::Pixel::MASK);
		for (int y = 0; y < vWorldSize.y; y++) {
			for (int x = 0; x < vWorldSize.x; x++) {
				olc::vi2d vWorld = ToScreen(x ,y);

				switch (pWorld[y * vWorldSize.x + x]) {
					case 0:
						// Invisible Tile
						DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 1 * vTileSize.x, 0, vTileSize.x, vTileSize.y);
				}
			}
		}
		SetPixelMode(olc::Pixel::NORMAL);

		return true;
	}
};

int main() {
	IsometricDemo demo;
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();
	return 0;
}
