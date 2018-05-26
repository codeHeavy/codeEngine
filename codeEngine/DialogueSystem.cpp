//DialogueSystem.cpp
//By Joshua Rand

#include "DialogueSystem.h"

DialogueSystem::DialogueSystem(ID3D11Device* device, ID3D11DeviceContext* context, ResourceManager *rm, unsigned int width, unsigned int height) {
	this->device = device;
	this->context = context;
	spriteBatch.reset(new SpriteBatch(context));
#ifdef CE_DEBUG
	spriteFont.reset(new SpriteFont(device, L"../../Assets/Fonts/Calibri.spritefont"));
#else
	spriteFont.reset(new SpriteFont(device, L"Assets/Fonts/Calibri.spritefont"));
#endif
	currentLineIndex = 0;
	text = "";
	started = false;
	endOfLine = false;

	this->rm = rm;
	this->width = width;
	this->height = height;
	leftPortraitFilename = "";
	rightPortraitFilename = "";

	//For testing
	//lines.push_back("Hello. This is a test of the dialogue\nsystem.");
	//lines.push_back("Hi. This is another line to be spoken by an NPC.");
	//lines.push_back("This is a third line.");
	//currentChars = 0;
	//maxChars = lines[0].size();
}

DialogueSystem::~DialogueSystem() {
	delete leftPortrait;
	delete rightPortrait;
}

//Replace "\n" characters that appear in the input file (which become "\\n") with actual line breaks
void CheckForLineBreak(string &str) {
	for (int i = 0; i < str.length() - 1; i++) {
		if (str[i] == '\\' && str[i + 1] == 'n') {
			str = str.replace(str.begin() + i, str.begin() + i + 2, "\n");
		}
	}
}

//Load lines from a file
void DialogueSystem::LoadLinesFromFile(string filename) {
	ifstream file;
	file.open(filename);
	if (!file.good())
		return;

	while (file) {
		string line;
		if (!getline(file, line)) {
			break;
		}
		if (line.substr(0, 13) == "LeftPortrait=") {
			leftPortraitFilename = line.erase(0, 13);
			continue;
		}
		else if (line.substr(0, 14) == "RightPortrait=") {
			rightPortraitFilename = line.erase(0, 14);
			continue;
		}
		else {
			CheckForLineBreak(line);
			lines.push_back(line);
		}
	}
	file.close();

	currentChars = 0;
	maxChars = lines[0].size();

	if (leftPortraitFilename != "" && rightPortraitFilename != "") {
		LoadPortraits();
	}
}

void DialogueSystem::LoadPortraits() {
	wstring wstr1 = wstring(leftPortraitFilename.begin(), leftPortraitFilename.end());
	wstring wstr2 = wstring(rightPortraitFilename.begin(), rightPortraitFilename.end());

	leftPortrait = new Portrait(rm->GetTextureByName(wstr1)->GetSRV());
	rightPortrait = new Portrait(rm->GetTextureByName(wstr2)->GetSRV());

	leftPortrait->SetPosition(XOFFSET, height - leftPortrait->GetHeight() - YOFFSET);
	rightPortrait->SetPosition(width - rightPortrait->GetWidth() - XOFFSET, height - rightPortrait->GetHeight() - YOFFSET);
	//leftPortrait = rm->GetTextureByName(wstr1)->GetSRV();
	//rightPortrait = rm->GetTextureByName(wstr2)->GetSRV();
}

void DialogueSystem::Update(float deltaTime) {
	if (!started) return;
	if (currentChars < maxChars) {
		currentChars += deltaTime * CHAR_RATE;
		text = lines[currentLineIndex].substr(0, (int)currentChars);
		endOfLine = false;
	}
	else {
		text = lines[currentLineIndex];
		endOfLine = true;
	}
}

void DialogueSystem::DrawPortraits() {
	spriteBatch->Draw(rm->GetTextureByName(L"Black.png")->GetSRV(), XMFLOAT2(0, 600), Colors::White);
	spriteBatch->Draw(leftPortrait->GetSRV(), XMFLOAT2(leftPortrait->GetX(), leftPortrait->GetY()), Colors::White);
	spriteBatch->Draw(rightPortrait->GetSRV(), XMFLOAT2(rightPortrait->GetX(), rightPortrait->GetY()), Colors::White);
}

void DialogueSystem::Draw() {
	wstring wstr = wstring(text.begin(), text.end());
	CommonStates states(device);
	spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());
	//spriteBatch->Begin();
	if (leftPortrait != nullptr && rightPortrait != nullptr && started) {
		DrawPortraits();
	}
	spriteFont->DrawString(spriteBatch.get(), wstr.c_str(), Vector2(300.0f, 625.0f), Colors::White);

	spriteBatch->End();

	//Reset the blend state and depth stencil state after drawing the Sprite Batch
	context->OMSetBlendState(0, 0, 0xFFFFFFFF);
	context->OMSetDepthStencilState(0, 0);
}

//Similar to Undertale, one button starts dialogue, another button skips dialogue
void DialogueSystem::SkipButton() {
	if (!endOfLine && started) {
		endOfLine = true;
		currentChars = maxChars;
	}
}

//A state machine may have been useful
void DialogueSystem::NextButton() {
	if (lines.size() == 0) return;
	started = true;
	if (endOfLine && currentLineIndex < lines.size() - 1) {
		endOfLine = false;
		currentChars = 0;
		maxChars = lines[++currentLineIndex].size();
	}
	else if (endOfLine && currentLineIndex == lines.size() - 1) {
		started = false;
		currentChars = 0;
		text = "";
	}
}

void DialogueSystem::UpdateWidthAndHeight(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
}

bool DialogueSystem::Active() {
	return started;
}