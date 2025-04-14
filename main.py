"""
______________________________________________________________________________________

Ray Poulton
Main file for the Baffle Game C++ code generation from an AI.
This project gives an AI a pdf file with the instructions for a Baffle Game project,
combined with my prompting for best results.
The AI will then generate the C++ code for the game and output it to a file.
_______________________________________________________________________________________
"""


from dotenv import load_dotenv
from langchain_community.document_loaders import PyPDFLoader
from langchain_openai import ChatOpenAI
from langchain_core.messages import HumanMessage
from langgraph.graph import START, MessagesState, StateGraph
from langgraph.checkpoint.memory import MemorySaver
from langchain_core.prompts import ChatPromptTemplate, MessagesPlaceholder

load_dotenv()

# load environmnet variable:
# OPENAI_API_KEY

memory = MemorySaver()

model = ChatOpenAI(model="gpt-4o", temperature=0, max_tokens=4096)

# Load the path to the PDF file
file_path = "C:\\Documents\\Project_2\\Project_2.pdf"
loader = PyPDFLoader(file_path)
docs = loader.load()

# Extract full document content
full_content = "\n".join([doc.page_content for doc in docs])

# Specific prompt engineering for generating the Baffle game cpp code
prompt_template = ChatPromptTemplate.from_messages(
    [
        (
            "system",
            """
            You are to create every needed function and class, and ensure that the code runs with no errors.
            Create a complete implementation of the Baffle game according to the specifications provided from:
            LASER PHYSICS:
                - Entry at numbered edge point (0-39)
                - Travels straight until hitting baffle or edge
                - Deflection rules:
                LEFT BAFFLE (\\):
                    → RIGHT → DOWN
                    → LEFT → UP
                    → UP → RIGHT
                    → DOWN → LEFT
                
                RIGHT BAFFLE (/):
                    → RIGHT → UP
                    → LEFT → DOWN
                    → UP → LEFT
                    → DOWN → RIGHT

            LASER MOVEMENT ALGORITHM:

            1. Determine initial direction based on entry point:
            - Entry 0-9 (Left edge): RIGHT
            - Entry 10-19 (Top edge): DOWN
            - Entry 20-29 (Right edge): LEFT
            - Entry 30-39 (Bottom edge): UP

            2. Convert entry point to starting (row, col) position:
            - Left edge (0-9): row = 9 - entry, col = -1
            - Top edge (10-19): row = -1, col = entry - 10
            - Right edge (20-29): row = 29 - entry, col = 10
            - Bottom edge (30-39): row = 10, col = 39 - entry

            3. Move laser step-by-step until exit:
            while within bounds:
                a. Move one step in current direction
                    - RIGHT: col++
                    - LEFT: col--
                    - UP: row--
                    - DOWN: row++
                
                b. Check if hit baffle:
                    if current cell contains LEFTBAFFLE or RIGHTBAFFLE:
                        apply deflection rules
                
                c. Check if exited:
                    if row < 0: exit at (10 + col)        // Top edge
                    if row >= 10: exit at (39 - col)      // Bottom edge
                    if col < 0: exit at (9 - row)         // Left edge
                    if col >= 10: exit at (20 + row)      // Right edge

            4. BAFFLE DEFLECTION RULES:
            LEFT BAFFLE (\\):
                Current Direction → New Direction
                RIGHT → DOWN
                LEFT → UP
                UP → RIGHT
                DOWN → LEFT
            
            RIGHT BAFFLE (/):
                Current Direction → New Direction
                RIGHT → UP
                LEFT → DOWN
                UP → LEFT
                DOWN → RIGHT

            5. Exit point conversion:
            - Top edge (0-9): 10 + col
            - Right edge (10-19): 20 + row
            - Bottom edge (20-29): 39 - col
            - Left edge (30-39): 9 - row

            Baffle Game Board Example:
            Ensure the board looks exactly like this, and is formatted correctly:

            10  11  12  13  14  15  16  17  18  19
            +---+---+---+---+---+---+---+---+---+---+
            9|   |   |   | \\ |   |   |   |   |   |   |20
            +---+---+---+---+---+---+---+---+---+---+
            8|   | / |   |   |   |   |   |   |   |   |21
            +---+---+---+---+---+---+---+---+---+---+
            7|   |   |   |   |   | \\ |   |   |   |   |22
            +---+---+---+---+---+---+---+---+---+---+
            6|   |   | / |   |   |   |   |   |   |   |23
            +---+---+---+---+---+---+---+---+---+---+
            5|   |   |   |   |   |   |   | / |   |   |24
            +---+---+---+---+---+---+---+---+---+---+
            4|   |   |   |   | \\ |   |   |   |   |   |25
            +---+---+---+---+---+---+---+---+---+---+
            3|   |   |   |   |   |   |   |   | / |   |26
            +---+---+---+---+---+---+---+---+---+---+
            2|   | \\ |   |   |   |   |   |   |   |   |27
            +---+---+---+---+---+---+---+---+---+---+
            1|   |   |   | / |   |   |   |   |   |   |28
            +---+---+---+---+---+---+---+---+---+---+
            0|   |   |   |   |   |   |   |   |   |   |29
            +---+---+---+---+---+---+---+---+---+---+
            39  38  37  36  35  34  33  32  31  30

            Legend:
            \\ = LEFT baffle (deflects laser 90° clockwise)
            / = RIGHT baffle (deflects laser 90° counter-clockwise)
            Numbers = Entry/exit points (0-39)
            
            EDGE ENTRY SYSTEM (BOARD COORDINATES AND DIRECTIONS):
                - Entry 0-9 (Left edge) direction = RIGHT
                - Entry 10-19 (Top edge) direction = DOWN
                - Entry 20-29 (Right edge) direction = LEFT
                - Entry 30-39 (Bottom edge) direction = UP

            Example: If entry = 33, the laser will go straight up and exit at 16.
            Example: If entry = 4, the laser will go right and exit at 25.
            Example: If entry = 14, the laser will go down and exit at 35.

            REMEMBER THESE EXAMPLES WHEN CODING THE LASER LOGIC.

            ENSURE THE BOARD NUMBERS MATCH THE GUESSING AND LASER NUMBERS EXACTLY.        

            and the pdf file provided.

            REQUIREMENTS:
            You must output a COMPLETE C++ file. DO NOT leave any function unimplemented. DO NOT say 'implement this yourself' or 'you need to implement...'. 
            All functions must be fully coded. DO NOT stop until all game logic is implemented. DO NOT summarize. JUST the full C++ code.
            Ensure the game board looks identical to the one I showed you above. Use #include<iomanip> to format the board correctly.
            Be sure that the laser physics match the game board exactly, as well as the instructions I gave you.
            """
        ),
        MessagesPlaceholder(variable_name="messages"),
    ]
)

# Define a new state graph for messages
workflow = StateGraph(state_schema=MessagesState)

def call_model(state: MessagesState):
    prompt = prompt_template.invoke(state)
    response = model.invoke(prompt)
    return {"messages": response}

if __name__ == "__main__":
    """
    May need to be run multiple times to get the best result.
    The output is unpredictable at times, but should get the correct result within 5 tries.
    """
    print("Sending Instructions to AI...")
    print("Please wait...")

    workflow.add_edge(START, "model")
    workflow.add_node("model", call_model)

    # Add memory
    memory = MemorySaver()
    app = workflow.compile(checkpointer=memory)


    config = {"configurable": {"thread_id": "project_2"}}
    input_messages = [HumanMessage(content=full_content)]
    output = app.invoke({"messages": input_messages}, config)

    # Get the AI's response
    ai_response = output["messages"][-1].content

    # Save to a .cpp file
    output_file = "BaffleGame.cpp"
    with open(output_file, "w") as file:
        file.write(ai_response)

    print(f"Successfully saved the C++ code to {output_file}")

