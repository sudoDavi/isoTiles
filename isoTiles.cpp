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

		olc::vi2d vMouse{ GetMouseX(), GetMouseY() };
		olc::vi2d vCell{ vMouse.x / vTileSize.x, vMouse.y / vTileSize.y };
		olc::vi2d vOffset{ vMouse.x % vTileSize.x, vMouse.y % vTileSize.y };
		olc::Pixel col = sprIsom->GetPixel(3 * vTileSize.x + vOffset.x, vOffset.y);

		olc::vi2d vSelected{
			 (vCell.y - vOrigin.y) + (vCell.x - vOrigin.x),
			 (vCell.y - vOrigin.y) - (vCell.x - vOrigin.x)
		};

		if (col == olc::RED)    vSelected += {-1, +0};
		if (col == olc::BLUE)   vSelected += {+0, -1};
		if (col == olc::GREEN)  vSelected += {+0, +1};
		if (col == olc::YELLOW) vSelected += {+1, +0};

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
						DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 1 * vTileSize.x, 0 * vTileSize.y, vTileSize.x, vTileSize.y);
						break;
					case 1:
						// Grass Tile
						DrawPartialSprite(vWorld.x, vWorld.y, sprIsom, 2 * vTileSize.x, 0 * vTileSize.y, vTileSize.x, vTileSize.y);
						break;
					case 2:
						// Pine tree
						DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 0 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, 2 * vTileSize.y);
						break;
					case 3:
						// Dead tree
						DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 1 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, 2 * vTileSize.y);
						break;
					case 4:
						// Sand tile
						DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 2 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, 2 * vTileSize.y);
						break;
					case 5:
						// Water tile
						DrawPartialSprite(vWorld.x, vWorld.y - vTileSize.y, sprIsom, 3 * vTileSize.x, 1 * vTileSize.y, vTileSize.x, 2 * vTileSize.y);
						break;
				}
			}
		}

		SetPixelMode(olc::Pixel::ALPHA);
		olc::vi2d vSelectedWorld = ToScreen(vSelected.x, vSelected.y);
		

		if ((vSelected.x < vWorldSize.x) && (vSelected.y < vWorldSize.y) && (vSelected.x >= 0) && (vSelected.y >= 0)) {
			// Draws the selected tile sprite over the selected world cell
			DrawPartialSprite(vSelectedWorld.x, vSelectedWorld.y, sprIsom, 0 * vTileSize.x, 0, vTileSize.x, vTileSize.y);

			if (GetMouse(0).bPressed)
				// Set the seleted tile +1
				++pWorld[vSelected.x + (vSelected.y * vWorldSize.x)] %= 6;

			if (GetMouse(1).bPressed) {
				// Set the seleted tile -1
				--pWorld[vSelected.x + (vSelected.y * vWorldSize.x)];
				if (pWorld[vSelected.x + (vSelected.y * vWorldSize.x)] < 0)
					pWorld[vSelected.x + (vSelected.y * vWorldSize.x)] = 6;
			}
		}

		SetPixelMode(olc::Pixel::NORMAL);

		// Draws Red rectangle around the selected screen cell
		// DrawRect(vCell.x * vTileSize.x, vCell.y * vTileSize.y, vTileSize.x, vTileSize.y, olc::RED);
		
		DrawString(4, 4, "Mouse: " + std::to_string(vMouse.x) + ", " + std::to_string(vMouse.y), olc::BLACK);
		// Tells us what screen Cell is selected
		// DrawString(4, 14, "Cell : " + std::to_string(vCell.x) + ", " + std::to_string(vCell.y), olc::BLACK);
		DrawString(4, 24, "Selected : " + std::to_string(vSelected.x) + ", " + std::to_string(vSelected.y), olc::BLACK);
		DrawString(4, 34, "Tile : " + std::to_string(pWorld[vSelected.x + (vSelected.y * vWorldSize.x)]), olc::BLACK);
		DrawString(4, 44, "WIndex : " + std::to_string(vSelected.x + (vSelected.y * vWorldSize.x)), olc::BLACK);

		return true;
	}
};

int main() {
	IsometricDemo demo;
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();
	return 0;
}
