#ifndef CELL_HPP
#define CELL_HPP

#include <cstddef>
#include <cstdint>
#include <SDL2/SDL.h>

class Cell {
public:
    enum class Type : uint8_t {
        Hidden,
        Flag,
        Question,               // Unused
        TriggeredMine,
        FalseMine,
        Mine,
        Question2,
        Nearby8,
        Nearby7,
        Nearby6,
        Nearby5,
        Nearby4,
        Nearby3,
        Nearby2,
        Nearby1,
        Nearby0,
    };

    static constexpr size_t width { 16 };
    explicit Cell(const int x, const int y, Type type);
    explicit Cell() = default;
    Type type() const { return m_type; }
    bool flagged() const { return m_flagged; }
    bool revealed() const { return m_revealed; }
    void toggle_flag();
    void reveal();
    void set_type(Type type);
    void render(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer, Type type);
    static void set_texture(SDL_Texture*& texture);
    static SDL_Texture* get_texture() { return m_texture; }
    static Cell::Type nearby_mines_to_cell_type(const int nearby);
private:
    int m_x;
    int m_y;
    Type m_type { Type::Nearby0 };
    bool m_flagged { false };
    bool m_revealed { false };
    static SDL_Texture* m_texture;
};

#endif //CELL_HPP
