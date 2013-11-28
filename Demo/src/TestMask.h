#pragma once
#include "test.h"
#include "initActor.h"

class TestMask: public Test
{
public:
	timeMS _lastSnow;

	spMaskedSprite _masked;
	spSprite _mask;

	TestMask():_lastSnow(0)
	{		
		_mask = initActor(new Sprite, 
			arg_scale = 3,
			arg_alpha = 64,
			arg_x = 100,
			arg_y = 50,
			arg_anchor = Vector2(0.5f, 0.5f),
			arg_resAnim = resources.getResAnim("logo2"),
			arg_attachTo = content);

		_mask->addTween(Actor::TweenRotation(MATH_PI * 2), 15000, -1, true);
		_mask->addTween(Actor::TweenX(content->getWidth() - 100), 10000, -1, true);
		_mask->addTween(Actor::TweenY(content->getHeight() - 50), 12000, -1, true);

		_masked = initActor(new MaskedSprite,
			arg_attachTo = content);

		_masked->setMask(_mask);


		TextStyle style;
		style.font = resourcesUI.getResFont("main")->getFont();
		style.color = Color(0, 255, 50, 255);
		style.vAlign = TextStyle::VALIGN_MIDDLE;
		style.hAlign = TextStyle::HALIGN_CENTER;
		style.multiline = true;

		spTextActor text = initActor(new TextActor(), 
			arg_attachTo = _masked,
			arg_scale = 2,
			arg_pos = content->getSize()/2,
			arg_text = "Oxygine. Masked text",
			arg_style = style);		


		const Test::toggle sm[] = {Test::toggle("hide mask", 0), Test::toggle("show mask", 1), };
		addToggle("show_mask", sm, 2);

		const Test::toggle cm[] = {Test::toggle("change mask1", 0), Test::toggle("change mask2", 1), };
		addToggle("change_mask", cm, 2);

		const Test::toggle dm[] = {Test::toggle("disable mask", 0), Test::toggle("enable mask", 1), };
		addToggle("disable_mask", dm, 2);

		const Test::toggle pause[] = {Test::toggle("pause", 0), Test::toggle("resume", 1), };
		addToggle("pause", pause, 2);
	}

	void toggleClicked(string id, const toggle *t)
	{
		if (id == "show_mask")
			_mask->setVisible(t->value != 0);

		if (id == "disable_mask") 
		{
			if (t->value == 0)
				_masked->setMask(0);
			else
				_masked->setMask(_mask);
		}

		if (id == "pause")
		{
			spClock clock = getRoot()->getClock();
			if (t->value == 0)
				clock->pause();
			else
				clock->resume();
		}

		if (id == "change_mask") 
		{			
			_mask->removeTweensByName("tweenanim");
			switch(t->value)
			{
			case 1:
				_mask->setResAnim(resources.getResAnim("logo2"));
				break;
			case 0:
				_mask->addTween(TweenAnim(resources.getResAnim("anim")), 600, -1, false)->setName("tweenanim");
				break;
			}			
		}
	}


	void doUpdate(const UpdateState &us)
	{
		if (_lastSnow  + 40 < us.time)
		{
			_lastSnow = us.time;
			spSprite snow = initActor(new Sprite,
				arg_anchor = Vector2(0.5f, 0.5f),
				arg_resAnim = resources.getResAnim("snow"),
				arg_attachTo = _masked,
				arg_rotation = scalar::randFloat(0, MATH_PI * 2),
				arg_y = -50,
				arg_scale = scalar::randFloat(0.4f, 1.0f),
				arg_x = scalar::randFloat(0, content->getWidth()));

			snow->addTween(Actor::TweenY(content->getHeight() + 50), 6000)->setDetachActor(true);
			snow->addTween(Actor::TweenRotation(scalar::randFloat(0, MATH_PI * 2)), 10000);
		}
	}
};
