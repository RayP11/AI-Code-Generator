# Baffle Game Project

This project automates the generation of a C++ implementation of the Baffle Game using AI from a single query.

## Files in the Project

### Python Files
- **`main.py`**: The main Python script that:
  - Loads the PDF instructions (`Project_2.pdf`).
  - Uses LangChain and OpenAI's GPT-4o model to generate the C++ code.
  - Saves the generated code to `BaffleGame.cpp`.

### Input File
- **`Project_2.pdf`**: A PDF file containing the instructions and rules for the Baffle Game.

### Output Files
- **`BaffleGame.cpp`**: The AI-generated C++ implementation of the Baffle Game.
- **`BestRun.cpp`**: The best generated AI code from trial runs

### Configuration Files
- **`.env`**: Contains the OpenAI API key required for the script.
- **`requirements.txt`**: Lists the Python dependencies for the project.

## How It Works

1. **PDF Loading**:
   - The script uses `PyPDFLoader` to extract the content of `Project_2.pdf`.

2. **Prompt Engineering**:
   - I crafted a very specific prompt to guide the AI in generating the C++ code. The prompt includes:
     - Laser physics rules.
     - Board layout and formatting.
     - Additional game logic and requirements.

3. **AI Code Generation**:
   - The script sends the prompt and PDF content to OpenAI's GPT-4o model.
   - The AI generates a complete C++ implementation of the game.

4. **Output**:
   - The generated code is saved to `BaffleGame.cpp`.

## Prerequisites

- Python 3.8 or higher
- OpenAI API key (stored in `.env`)
- C++ compiler

## Setup

1. Clone the repository and navigate to the project directory.
2. Install Python dependencies:
   ```bash
   pip install -r requirements.txt