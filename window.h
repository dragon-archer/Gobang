/**
 * @file      window.h
 * @brief     Declartion of GUI window class
 * @version   0.1
 * @author    dragon-archer
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _GOBANG_WINDOW_H_
#define _GOBANG_WINDOW_H_

#include "pch_gui.h"
#include "gobang.h"
#include "ai.h"

constexpr Qt::GlobalColor player_color[3] = { Qt::red, Qt::black, Qt::white };

QT_BEGIN_NAMESPACE
namespace Ui {
	class Window;
}
QT_END_NAMESPACE

class Window : public QWidget {
	Q_OBJECT

public:
	Window(QWidget* parent = nullptr);
	~Window();

public Q_SLOTS:
	void onBoardItemClicked(int row, int col);
	void onBtnRestart();
	void onBtnUndo();

private:
	bool doMove(int x, int y, Player role);
	void updateSteps();
	void addTextToInfo(const QString& text);

private:
	Ui::Window* ui;
	Gobang		game { DEFAULT_SIZE, WIN_SIZE };
	bool		gameContinue { true };
};

#endif // _GOBANG_WINDOW_H_
