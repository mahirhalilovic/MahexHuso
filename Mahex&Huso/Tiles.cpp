#include "headers/Tiles.hpp"

namespace Tiles {
    HBITMAP m_tileBitmap = nullptr;
    HBITMAP m_tileMask = nullptr;
    HBITMAP m_spikesBitmap = nullptr;
    HBITMAP m_spikesMask = nullptr;
    HBITMAP m_coinBitmap = nullptr;
    HBITMAP m_coinMask = nullptr;
    HBITMAP m_keydownBlockBitmap = nullptr;
    HBITMAP m_keydownBlockMask = nullptr;
    HBITMAP m_pressureBlockBitmap = nullptr;
    HBITMAP m_pressureBlockMask = nullptr;
    HBITMAP m_pressureBlockActiveBitmap = nullptr;
    HBITMAP m_pressureBlockActiveMask = nullptr;
    HBITMAP m_plateHorizontalBitmap = nullptr;
    HBITMAP m_plateHorizontalMask = nullptr;
    HBITMAP m_plateVerticalBitmap = nullptr;
    HBITMAP m_plateVerticalMask = nullptr;
    HBITMAP m_mahexEndBitmap = nullptr;
    HBITMAP m_mahexEndMask = nullptr;
    HBITMAP m_husoEndBitmap = nullptr;
    HBITMAP m_husoEndMask = nullptr;

    struct BitmapLoader {
        BitmapLoader() {
            m_tileBitmap = (HBITMAP) LoadImage(NULL, L"assets/images/tile.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_tileMask = (HBITMAP) LoadImage(NULL, L"assets/images/tile_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_spikesBitmap = (HBITMAP) LoadImage(NULL, L"assets/images/spikes.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_spikesMask = (HBITMAP) LoadImage(NULL, L"assets/images/spikes_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_coinBitmap = (HBITMAP) LoadImage(NULL, L"assets/images/coin.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_coinMask = (HBITMAP) LoadImage(NULL, L"assets/images/coin_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_keydownBlockBitmap = (HBITMAP) LoadImage(NULL, L"assets/images/keydown.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_keydownBlockMask = (HBITMAP) LoadImage(NULL, L"assets/images/keydown_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_pressureBlockBitmap = (HBITMAP) LoadImage(NULL, L"assets/images/pressure.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_pressureBlockMask = (HBITMAP) LoadImage(NULL, L"assets/images/pressure_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_pressureBlockActiveBitmap = (HBITMAP) LoadImage(NULL, L"assets/images/pressure_active.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_pressureBlockActiveMask = (HBITMAP) LoadImage(NULL, L"assets/images/pressure_active_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_plateHorizontalBitmap = (HBITMAP) LoadImage(NULL, L"assets/images/horizontal_plate.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_plateHorizontalMask = (HBITMAP) LoadImage(NULL, L"assets/images/horizontal_plate_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_plateVerticalBitmap = (HBITMAP) LoadImage(NULL, L"assets/images/vertical_plate.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_plateVerticalMask = (HBITMAP) LoadImage(NULL, L"assets/images/vertical_plate_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_mahexEndBitmap = (HBITMAP) LoadImage(NULL, L"assets/images/door_mahex.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_mahexEndMask = (HBITMAP) LoadImage(NULL, L"assets/images/door_mahex_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_husoEndBitmap = (HBITMAP) LoadImage(NULL, L"assets/images/door_huso.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            m_husoEndMask = (HBITMAP) LoadImage(NULL, L"assets/images/door_huso_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

            if(!m_tileBitmap || !m_tileMask || !m_spikesBitmap || !m_spikesMask || !m_coinBitmap || !m_coinMask ||
                !m_keydownBlockBitmap || !m_keydownBlockMask || !m_pressureBlockBitmap || !m_pressureBlockMask ||
                !m_pressureBlockActiveBitmap || !m_pressureBlockActiveMask ||
                !m_plateHorizontalBitmap || !m_plateHorizontalMask || !m_plateVerticalBitmap || !m_plateVerticalMask ||
                !m_mahexEndBitmap || !m_mahexEndMask || !m_husoEndBitmap || !m_husoEndMask) {
                MessageBox(NULL, L"Failed to load one or more tile images", L"Error", MB_OK);
            }
        }
    };

    static BitmapLoader loader;
}