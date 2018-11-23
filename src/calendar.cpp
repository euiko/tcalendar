#include <cppurses/cppurses.hpp>
#include <signals/signals.hpp>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
using namespace cppurses;

// Button Widget that emits a signal when clicked.
class Button : public Widget {
   public:
    Button(Glyph_string title) : title_{std::move(title)} {}

    void set_title(Glyph_string title) {
        title_ = std::move(title);
        this->update();  // Post a Paint_event to the event queue.
    }

    // Signals
    sig::Signal<void()> clicked;

   protected:
    bool mouse_press_event(const Mouse_data& mouse) override {
        // Emit Signal if Left Button Pressed
        if (mouse.button == Mouse_button::Left) {
            clicked();
        }
        return Widget::mouse_press_event(mouse);
    }

    bool paint_event() override {
        // Calculate Center Position
        std::size_t x{(this->width() / 2) - (title_.length() / 2)};
        std::size_t y{this->height() / 2};
        Point position{x, y};

        // Put Title to Screen
        Painter p{this};
        p.put(title_, position);

        return Widget::paint_event();
    }

   private:
    Glyph_string title_;
};

struct Side_pane : public Vertical_layout {
    Textbox& tbox_mirror{this->make_child<Textbox>("Mirror")};
    Button& exit_btn{
        this->make_child<Button>(Glyph_string{"Exit", Attribute::Bold})};

    Side_pane() {
        enable_border(tbox_mirror);
        set_background(tbox_mirror, Color::Light_gray);
        set_foreground(tbox_mirror, Color::Dark_blue);

        exit_btn.height_policy.type(Size_policy::Fixed);
        exit_btn.height_policy.hint(3);
        set_background(exit_btn, Color::Dark_blue);
        set_foreground(exit_btn, Color::Light_gray);

        exit_btn.clicked.connect(System::quit);
    }
};

// Two Textboxes and Button to exit.
struct MainColumnLayout : public Horizontal_layout {
    public:
    Labeled_cycle_box& month_cycle_stack{this->make_child<Labeled_cycle_box>("Month")};
    Labeled_cycle_box& year_cycle_stack{this->make_child<Labeled_cycle_box>("Year")};
    Cycle_box& cbox{this->make_child<Cycle_box>()};
    Side_pane& side_pane{this->make_child<Side_pane>()};
    std::vector<Glyph_string> m_Months{
        "January", "February", "Maret", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"
    };
    public:
    MainColumnLayout() {
        Glyph_string a{"Halo dunia"};
        Glyph_string b{"Apa kabar"};
        Glyph_string c{"Baik baik saja"};
        cbox.width_policy.stretch(2);
        cbox.add_option(a);
        cbox.add_option(b);
        cbox.add_option(c);
        month_cycle_stack.width_policy.stretch(1);
        month_cycle_stack.cycle_box.option_changed.connect([this](std::string option) {
            Glyph_string gOption{option};
            std::vector<Glyph_string>::iterator it = std::find(m_Months.begin(), m_Months.end(), gOption);
            
            int index = std::distance(m_Months.begin(), it);
            side_pane.tbox_mirror.set_text(std::to_string(index));
            
            
        });
        for(Glyph_string& month : m_Months) {
            month_cycle_stack.cycle_box.add_option(month);
        }
        for(int year = 1; year < 3000; year++) {
            Glyph_string gYear{std::to_string(year)};
            year_cycle_stack.cycle_box.add_option(gYear);
        }
    }
};

struct MainRowLayout : public Vertical_layout {
    public:
    Titlebar& titlebar{this->make_child<Titlebar>("TUI - Calendar")};
    MainColumnLayout& main_panel{this->make_child<MainColumnLayout>()};

    MainRowLayout() {
        this->set_name("Main_menu - head widget");
        titlebar.set_name("Titlebar in Main_menu");
    };
};

int main() {
    System sys;

    MainRowLayout mainLayout;
    sys.set_head(&mainLayout);
    Focus::set_focus_to(&mainLayout.main_panel);

    return sys.run();
}