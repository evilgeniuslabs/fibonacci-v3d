/*
   Fibonacci v3D: https://github.com/evilgeniuslabs/fibonacci-v3d
   Copyright (C) 2014-2016 Jason Coon, Evil Genius Labs

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

class Cell
{
  public:
    byte alive = 1;
    byte prev = 1;
    byte hue = 6;
    byte brightness;
};

Cell world[kMatrixWidth][kMatrixHeight];

uint8_t neighbors(uint8_t x, uint8_t y)
{
  return (world[(x + 1) % kMatrixWidth][y].prev) +
         (world[x][(y + 1) % kMatrixHeight].prev) +
         (world[(x + kMatrixWidth - 1) % kMatrixWidth][y].prev) +
         (world[x][(y + kMatrixHeight - 1) % kMatrixHeight].prev) +
         (world[(x + 1) % kMatrixWidth][(y + 1) % kMatrixHeight].prev) +
         (world[(x + kMatrixWidth - 1) % kMatrixWidth][(y + 1) % kMatrixHeight].prev) +
         (world[(x + kMatrixWidth - 1) % kMatrixWidth][(y + kMatrixHeight - 1) % kMatrixHeight].prev) +
         (world[(x + 1) % kMatrixWidth][(y + kMatrixHeight - 1) % kMatrixHeight].prev);
}

void randomFillWorld()
{
  static uint8_t lifeDensity = 10;

  for (uint8_t i = 0; i < kMatrixWidth; i++) {
    for (uint8_t j = 0; j < kMatrixHeight; j++) {
      if (random(100) < lifeDensity) {
        world[i][j].alive = 1;
        world[i][j].brightness = 255;
      }
      else {
        world[i][j].alive = 0;
        world[i][j].brightness = 0;
      }
      world[i][j].prev = world[i][j].alive;
      world[i][j].hue = 0;
    }
  }
}

uint8_t life()
{
  static uint8_t generation = 0;

  // Display current generation
  for (uint8_t i = 0; i < kMatrixWidth; i++)
  {
    for (uint8_t j = 0; j < kMatrixHeight; j++)
    {
      setPixelXY(i, j, ColorFromPalette(currentPalette, world[i][j].hue * 4, world[i][j].brightness, LINEARBLEND));
    }
  }

  uint8_t liveCells = 0;

  // Birth and death cycle
  for (uint8_t x = 0; x < kMatrixWidth; x++)
  {
    for (uint8_t y = 0; y < kMatrixHeight; y++)
    {
      // Default is for cell to stay the same
      if (world[x][y].brightness > 0 && world[x][y].prev == 0)
        world[x][y].brightness *= 0.5;

      uint8_t count = neighbors(x, y);

      if (count == 3 && world[x][y].prev == 0)
      {
        // A new cell is born
        world[x][y].alive = 1;
        world[x][y].hue += 2;
        world[x][y].brightness = 255;
      }
      else if ((count < 2 || count > 3) && world[x][y].prev == 1)
      {
        // Cell dies
        world[x][y].alive = 0;
      }

      if (world[x][y].alive)
        liveCells++;
    }
  }

  // Copy next generation into place
  for (uint8_t x = 0; x < kMatrixWidth; x++)
  {
    for (uint8_t y = 0; y < kMatrixHeight; y++)
    {
      world[x][y].prev = world[x][y].alive;
    }
  }

  if (liveCells < 4 || generation >= 128)
  {
    fill_solid(leds, NUM_LEDS, CRGB::Black);

    randomFillWorld();

    generation = 0;
  }
  else
  {
    generation++;
  }

  return 60;
}

