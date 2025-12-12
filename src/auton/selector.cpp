#include "auton.h"
#include "main.h"
#include "subsystem/drivetrain.h"

// --- GLOBALS ---
enum AutonMode { LEFT_SIDE, RIGHT_SIDE, SKILLS, SOLO_AWP };

AutonMode selectedAuton = LEFT_SIDE;
Alliance currentAlliance = RED; // Default Alliance is RED

// Auton Selection Buttons
static lv_obj_t *leftBtn;
static lv_obj_t *rightBtn;
static lv_obj_t *skillsBtn;
static lv_obj_t *soloAwpBtn;

// Menu & Navigation Objects
static lv_obj_t *tabView;
static lv_obj_t *menuContainer;
static lv_obj_t *menuBtn;

// Alliance Selector
static lv_obj_t *allianceBtn; // This is now the clickable button OVER the image
static lv_obj_t *allianceLabel; // Label to display RED/BLUE
static lv_style_t styleBorderRed;
static lv_style_t styleBorderBlue;

// Styles
static lv_style_t styleBtnDefault;
static lv_style_t styleBtnChecked;
static lv_style_t styleMenuContainer;

// Debug Labels
static lv_obj_t *poseXLabel;
static lv_obj_t *poseYLabel;
static lv_obj_t *poseThetaLabel;

// --- ODOMETRY TIMER ---
void odomUpdateTimer(lv_timer_t *timer) {
  char buffer[100];
  if (poseXLabel != NULL) {
    sprintf(buffer, "X: %.2f", chassis.getPose().x);
    lv_label_set_text(poseXLabel, buffer);
  }
  if (poseYLabel != NULL) {
    sprintf(buffer, "Y: %.2f", chassis.getPose().y);
    lv_label_set_text(poseYLabel, buffer);
  }
  if (poseThetaLabel != NULL) {
    sprintf(buffer, "Theta: %.2f", formatHeading(chassis.getPose().theta));
    lv_label_set_text(poseThetaLabel, buffer);
  }
}

// --- ALLIANCE UPDATE TIMER ---
void allianceUpdateTimer(lv_timer_t *timer) {
  lv_obj_t *screen = lv_scr_act();

  // Remove both styles first to ensure a clean switch
  lv_obj_remove_style(screen, &styleBorderRed, LV_STATE_DEFAULT);
  lv_obj_remove_style(screen, &styleBorderBlue, LV_STATE_DEFAULT);

  // Apply the new border style
  if (currentAlliance == RED) {
    lv_obj_add_style(screen, &styleBorderRed, LV_STATE_DEFAULT);
  } else {
    lv_obj_add_style(screen, &styleBorderBlue, LV_STATE_DEFAULT);
  }
}

// --- VISUAL HELPERS ---
static void updateButtonStyles(lv_obj_t *selected) {
  // Reset all to default
  lv_obj_remove_style(leftBtn, &styleBtnChecked, LV_STATE_DEFAULT);
  lv_obj_remove_style(rightBtn, &styleBtnChecked, LV_STATE_DEFAULT);
  lv_obj_remove_style(skillsBtn, &styleBtnChecked, LV_STATE_DEFAULT);
  lv_obj_remove_style(soloAwpBtn, &styleBtnChecked, LV_STATE_DEFAULT);

  lv_obj_add_style(selected, &styleBtnChecked, LV_STATE_DEFAULT);
}

// --- CALLBACKS ---

// 1. Handles selecting the auton mode
static void modeButtonCallback(lv_event_t *e) {
  lv_obj_t *btn = lv_event_get_target(e);

  if (btn == leftBtn)
    selectedAuton = LEFT_SIDE;
  else if (btn == rightBtn)
    selectedAuton = RIGHT_SIDE;
  else if (btn == skillsBtn)
    selectedAuton = SKILLS;
  else if (btn == soloAwpBtn)
    selectedAuton = SOLO_AWP;

  updateButtonStyles(btn);
}

// 2. Handles opening/closing the sidebar menu
static void toggleMenuCallback(lv_event_t *e) {
  if (lv_obj_has_flag(menuContainer, LV_OBJ_FLAG_HIDDEN)) {
    lv_obj_clear_flag(menuContainer, LV_OBJ_FLAG_HIDDEN); // Show
  } else {
    lv_obj_add_flag(menuContainer, LV_OBJ_FLAG_HIDDEN); // Hide
  }
}

// 3. Handles switching tabs (Auton/Debug/Config) from the sidebar
static void tabSwitchCallback(lv_event_t *e) {
  // Get the index passed as user_data
  int tabIndex = (int)(uintptr_t)lv_event_get_user_data(e);

  // Switch the tabview
  lv_tabview_set_act(tabView, tabIndex, LV_ANIM_ON);

  // Auto-hide the menu after selection for better UX
  lv_obj_add_flag(menuContainer, LV_OBJ_FLAG_HIDDEN);
}

// 4. Handles switching alliance
static void allianceButtonCallback(lv_event_t *e) {
  // Toggle the alliance state and update the label
  if (lv_event_get_code(e) == LV_EVENT_LONG_PRESSED) {
    {
     if (currentAlliance == RED) {
       currentAlliance = BLUE;
       lv_label_set_text(allianceLabel, "BLUE");
     }
     else {
    currentAlliance = RED;
    lv_label_set_text(allianceLabel, "RED");
      }
    }
  }
}

// --- MAIN GUI SETUP ---
void gui() {
  lv_obj_t *screen = lv_scr_act();

  // 1. Scrolling Fix: Remove scrollable flag from the main screen
  lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

  // 2. INIT STYLES
  lv_style_init(&styleBtnDefault);
  lv_style_set_bg_color(&styleBtnDefault, lv_color_hex(0x1A1A1A));
  lv_style_set_border_width(&styleBtnDefault, 2);
  lv_style_set_border_color(&styleBtnDefault, lv_color_hex(0x444444));
  lv_style_set_radius(&styleBtnDefault, 8);
  lv_style_set_text_color(&styleBtnDefault, lv_color_hex(0xFFFFFF));

  lv_style_init(&styleBtnChecked);
  lv_style_set_bg_color(&styleBtnChecked, lv_color_hex(0xFFD700)); // Gold
  lv_style_set_border_color(&styleBtnChecked, lv_color_hex(0xFFD700));
  lv_style_set_text_color(&styleBtnChecked,
                          lv_color_hex(0x000000)); // Black text on gold

  lv_style_init(&styleMenuContainer);
  lv_style_set_bg_color(&styleMenuContainer, lv_color_hex(0x333333));
  lv_style_set_bg_opa(&styleMenuContainer, LV_OPA_90); // Slight transparency
  lv_style_set_border_width(&styleMenuContainer, 0);
  lv_style_set_radius(&styleMenuContainer, 0);

  // Alliance Border Styles
  const lv_coord_t ALLIANCE_BORDER_WIDTH = 8;

  lv_style_init(&styleBorderRed);
  lv_style_set_border_width(&styleBorderRed, ALLIANCE_BORDER_WIDTH);
  lv_style_set_border_color(&styleBorderRed, lv_color_hex(0xFF0000));
  lv_style_set_border_side(&styleBorderRed,
                           LV_BORDER_SIDE_FULL); // Apply to all sides

  lv_style_init(&styleBorderBlue);
  lv_style_set_border_width(&styleBorderBlue, ALLIANCE_BORDER_WIDTH);
  lv_style_set_border_color(&styleBorderBlue, lv_color_hex(0x0000FF));
  lv_style_set_border_side(&styleBorderBlue, LV_BORDER_SIDE_FULL);

  // 3. CREATE TABVIEW (The main container)
  tabView = lv_tabview_create(screen, LV_DIR_TOP, 0);
  lv_obj_set_style_bg_color(tabView, lv_color_hex(0x000000), LV_STATE_DEFAULT);

  lv_obj_t *autonsTab = lv_tabview_add_tab(tabView, "Autons");
  lv_obj_t *debugTab = lv_tabview_add_tab(tabView, "Debug");
  lv_obj_t *configTab = lv_tabview_add_tab(tabView, "Config");

  // Force black backgrounds & Scrolling Fix 2: Remove scrollable flag from tab
  // contents
  lv_obj_set_style_bg_color(autonsTab, lv_color_hex(0x000000),
                            LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(debugTab, lv_color_hex(0x000000), LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(configTab, lv_color_hex(0x000000),
                            LV_STATE_DEFAULT);

  lv_obj_set_width(autonsTab, lv_pct(100));
  lv_obj_set_height(autonsTab, lv_pct(100));
  lv_obj_set_width(debugTab, lv_pct(100));
  lv_obj_set_height(debugTab, lv_pct(100));
  lv_obj_set_width(configTab, lv_pct(100));
  lv_obj_set_height(configTab, lv_pct(100));
  
  lv_obj_clear_flag(autonsTab, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_clear_flag(debugTab, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_clear_flag(configTab, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_scrollbar_mode(autonsTab, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_scrollbar_mode(debugTab, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_scrollbar_mode(configTab, LV_SCROLLBAR_MODE_OFF);
  // ==========================================
  // TAB 1: AUTONS
  // ==========================================

  // Main Container for this tab
  lv_obj_t *autonCont = lv_obj_create(autonsTab);
  lv_obj_set_size(autonCont, 480, 240);
  lv_obj_set_style_bg_color(autonCont, lv_color_hex(0x000000), 0);
  lv_obj_set_style_border_width(autonCont, 0, 0);
  lv_obj_set_style_pad_all(autonCont, 0, 0);
  lv_obj_set_flex_flow(autonCont,
                       LV_FLEX_FLOW_ROW); // Puts Image next to Button Column

  // -- LEFT: Image & Clickable Area (Container to hold image and button
  // overlay) --

  // Image Container: Positioned exactly where the image should go
  lv_obj_t *imageContainer = lv_obj_create(autonCont);
  lv_obj_set_size(imageContainer, 220, 240); // Image size
  lv_obj_set_style_bg_opa(imageContainer, LV_OPA_0,
                          0); // Make background transparent
  lv_obj_set_style_border_width(imageContainer, 0, 0);
  lv_obj_set_style_pad_all(imageContainer, 0, 0);

  // Position adjustments
  lv_obj_set_style_align(imageContainer, LV_ALIGN_TOP_LEFT, 0);
  lv_obj_set_style_pad_left(imageContainer,-20, 0);
  lv_obj_set_style_pad_top(imageContainer, -45,0); 

  // The actual Image
  LV_IMG_DECLARE(christmas_transparent);
  lv_obj_t *previewImage = lv_img_create(imageContainer);
  lv_img_set_src(previewImage, &christmas_transparent);
  lv_obj_set_align(previewImage, LV_ALIGN_CENTER);

  // The transparent Button OVER the image
  allianceBtn = lv_btn_create(imageContainer);
  lv_obj_set_size(allianceBtn, 240, 240);
  lv_obj_set_style_bg_opa(allianceBtn, LV_OPA_0, 0); // Transparent background
  lv_obj_set_style_border_width(allianceBtn, 0, 0);
  lv_obj_add_event_cb(allianceBtn, allianceButtonCallback, LV_EVENT_LONG_PRESSED,
                      NULL);

  // Label for RED/BLUE text (created as a child of the transparent button)
  allianceLabel = lv_label_create(allianceBtn);
  lv_label_set_text(allianceLabel, "RED"); // Initial state
  lv_obj_set_style_text_font(allianceLabel, &lv_font_montserrat_20, 0);
  lv_obj_set_style_align(allianceLabel, LV_ALIGN_BOTTOM_LEFT, 0);

  // Add margin to push the text 5 pixels away from the left and bottom edges.
  lv_obj_set_style_pad_left(allianceLabel, 5, 0);
  lv_obj_set_style_pad_bottom(allianceLabel, 5, 0);

  // Ensure the label content does not inherit center alignment
  lv_obj_set_style_text_align(allianceLabel, LV_TEXT_ALIGN_LEFT, 0);

  // -- RIGHT: Button Column (1x4) --
  lv_obj_t *buttonColumn = lv_obj_create(autonCont);
  lv_obj_set_flex_grow(buttonColumn, 1); // Fill remaining width
  lv_obj_set_height(buttonColumn, 240);
  lv_obj_set_style_bg_color(buttonColumn, lv_color_hex(0x000000), 0);
  lv_obj_set_style_border_width(buttonColumn, 0, 0);

  // Position shift (20px up, 40px left)
  lv_obj_set_style_pad_top(buttonColumn, -20, 0);
  lv_obj_set_style_pad_left(buttonColumn, -50, 0);

  // Set to COLUMN flow (1x4)
  lv_obj_set_flex_flow(buttonColumn, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(buttonColumn, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_row(buttonColumn, 8, 0);

  // Helper to create auton buttons
  auto create_auton_btn = [&](const char *txt, lv_obj_t **btnObj) {
    *btnObj = lv_btn_create(buttonColumn);
    // Width 180px, Height 45px (Fits 4 in a column)
    lv_obj_set_size(*btnObj, 180, 45);
    lv_obj_add_style(*btnObj, &styleBtnDefault, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(*btnObj, modeButtonCallback, LV_EVENT_CLICKED, NULL);

    lv_obj_t *l = lv_label_create(*btnObj);
    lv_label_set_text(l, txt);
    lv_obj_center(l);
  };
  
  create_auton_btn("LEFT SIDE", &leftBtn);
  create_auton_btn("RIGHT SIDE", &rightBtn);
  create_auton_btn("SKILLS", &skillsBtn);
  create_auton_btn("SOLO AWP", &soloAwpBtn);
  
  updateButtonStyles(leftBtn);

  lv_obj_move_foreground(buttonColumn);
  
  // ==========================================
  // TAB 2: DEBUG (Odom)
  // ==========================================
  lv_obj_t *odomContainer = lv_obj_create(debugTab);
  lv_obj_set_size(odomContainer, 400, 200);
  lv_obj_center(odomContainer);
  lv_obj_set_flex_flow(odomContainer, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_bg_color(odomContainer, lv_color_hex(0x000000), 0);

  poseXLabel = lv_label_create(odomContainer);
  poseYLabel = lv_label_create(odomContainer);
  poseThetaLabel = lv_label_create(odomContainer);
  lv_timer_create(odomUpdateTimer, 50, NULL);

  // ==========================================
  // GLOBAL MENU SYSTEM (Overlay)
  // ==========================================

  // 1. The Menu Toggle Button (Top Left)
  menuBtn = lv_btn_create(screen);
  lv_obj_set_size(menuBtn, 50, 40);
  lv_obj_align(menuBtn, LV_ALIGN_TOP_LEFT, 5, 5);
  lv_obj_add_event_cb(menuBtn, toggleMenuCallback, LV_EVENT_CLICKED, NULL);
  lv_obj_set_style_bg_color(menuBtn, lv_color_hex(0x444444), 0);

  lv_obj_t *menuLabel = lv_label_create(menuBtn);
  lv_label_set_text(menuLabel, LV_SYMBOL_LIST); // The "Hamburger" icon
  lv_obj_center(menuLabel);

  // 2. The Sidebar Container (Hidden by default)
  menuContainer = lv_obj_create(screen);
  lv_obj_set_size(menuContainer, 140, 240); // 140px wide sidebar
  lv_obj_align(menuContainer, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_add_style(menuContainer, &styleMenuContainer, 0);
  lv_obj_set_flex_flow(menuContainer, LV_FLEX_FLOW_COLUMN);
  lv_obj_add_flag(menuContainer, LV_OBJ_FLAG_HIDDEN); // Start hidden
  lv_obj_set_scrollbar_mode(menuContainer, LV_SCROLLBAR_MODE_OFF);

  // 3. Navigation Buttons inside Sidebar
  auto create_nav_btn = [&](const char *txt, int targetInfo) {
    lv_obj_t *b = lv_btn_create(menuContainer);
    lv_obj_set_width(b, lv_pct(100));
    lv_obj_set_height(b, 50);
    lv_obj_t *l = lv_label_create(b);
    lv_label_set_text(l, txt);
    lv_obj_center(l);
    // Pass the tab index (0, 1, or 2) as user data
    lv_obj_add_event_cb(b, tabSwitchCallback, LV_EVENT_CLICKED,
                        (void *)(uintptr_t)targetInfo);
  };

  create_nav_btn("AUTONS", 0);
  create_nav_btn("DEBUG", 1);
  create_nav_btn("CONFIG", 2);

  // Close button for the menu (optional, but good UX)
  lv_obj_t *closeBtn = lv_btn_create(menuContainer);
  lv_obj_set_width(closeBtn, lv_pct(100));
  lv_obj_set_style_bg_color(closeBtn, lv_color_hex(0xAA0000), 0); // Red
  lv_obj_t *closeLbl = lv_label_create(closeBtn);
  lv_label_set_text(closeLbl, "CLOSE");
  lv_obj_center(closeLbl);
  lv_obj_add_event_cb(closeBtn, toggleMenuCallback, LV_EVENT_CLICKED, NULL);

  // Start the timer to continually update the screen border
  lv_timer_create(allianceUpdateTimer, 100, NULL);
}

void autonSelector() {
  switch (selectedAuton) {
  case LEFT_SIDE:
    leftAuton();
    break;
  case RIGHT_SIDE:
    rightAuton();
    break;
  case SKILLS:
    skillsAuton();
    break;
  case SOLO_AWP:
    soloAWP();
    break;
  }
}