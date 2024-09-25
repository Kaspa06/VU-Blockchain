# Pseudo-kodas

    Algorithm inputToHash(input)
    // 1. Modifikuoti įvestį
    Create modifiedInput = input + input.length
    for each character c in modifiedInput
        if c is an uppercase letter OR c == '!'
            c = XOR with (position mod 10)
            c = Shift ASCII value up by 5
        else
            c = XOR with (position + 3)
            c = Shift ASCII value down by 2
    Reverse modifiedInput

    // 2. Konvertuoti į bitus
    while modifiedInput length < 32
        Append 'k' to modifiedInput

    Create binaryResult = ""
    for each character c in modifiedInput
        Convert c to 8-bit binary
        Append to binaryResult

    // 3. Skaičiuoti žodžių sumą
    Create wordSum = 0
    for each character c in modifiedInput
        if c is a letter
            Add alphabetical position of c (a = 1, b = 2, ...) to wordSum

    // 4. Modifikuoti bitus pagal žodžių sumą
    for each bit b in binaryResult
        if b == '1'
            if wordSum is even
                b = '0'
            else
                b = '1'
        else
            if wordSum is even
                b = '1'
            else
                b = '0'
        wordSum = wordSum / 2
        if wordSum == 0
            Break the loop

    // 5. Konvertuoti į šešioliktainę reikšmę
    Create hexResult = ""
    Create rollingValue = 1
    for each 4-bit chunk in binaryResult
        Convert chunk to an integer value
        rollingValue = rollingValue + (influence from corresponding character in modifiedInput)
        value = value * rollingValue * (multiplier based on input length) * non-linearity (sin function)
        Convert value to hexadecimal and append to hexResult

    // 6. Modifikuoti šešioliktainę reikšmę
    for each character c in hexResult
        Increment hex character c using helper function

    // 7. Užtikrinti, kad maiša būtų 64 simbolių ilgio
    if hexResult length > 64
        Truncate hexResult to 64 characters
    else
        Append '0' to hexResult until length is 64

    // 8. Grąžinti maišos reikšmę
    Return hexResult
