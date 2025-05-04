---

# 🖼️ Image Sharpening & Histogram

## 📌 Description

This project implements **convolution-based image processing** using a **sharpening mask**, followed by **grayscale conversion** and **brightness histogram generation**.
The program takes a **PPM image** as input, applies a **sharpening filter**, converts the image to **grayscale**, and computes a **brightness histogram**.

---

## 📂 Project Structure

📁 **File:**

* **`main.c`** — the main file that handles image processing.

---

## 🎯 Key Features

✅ **Loads a PPM image** into memory.
✅ **Applies a sharpening filter** with the following kernel:

```
0  -1   0  
-1  5  -1  
0  -1   0  
```

✅ **Converts the image to grayscale** using the formula:

```
Y = round(0.2126*R + 0.7152*G + 0.0722*B)
```

✅ **Generates a brightness histogram** divided into 5 ranges.
✅ **Saves results to** `output.ppm` (sharpened image) and `output.txt` (histogram data).

---

## 🛠 Usage

### **🔧 Build the Project**

```sh
gcc -o image_processor main.c -lm
```

### **▶️ Run the Program**

```sh
./image_processor input.ppm
```

The program expects a **PPM image** as the input file.

#### **Example**

```sh
$ ./image_processor vit_small.ppm
```

Output files:

* **`output.ppm`** — the processed image with enhanced sharpness
* **`output.txt`** — brightness histogram in the format:

  ```
  123 456 789 321 654
  ```

---

## 📊 Brightness Histogram

The histogram is split into **5 brightness ranges**:

| Bin | Pixel Brightness Range |
| --- | ---------------------- |
| 0   | 0 - 50                 |
| 1   | 51 - 101               |
| 2   | 102 - 152              |
| 3   | 153 - 203              |
| 4   | 204 - 255              |

The `output.txt` file contains **5 numbers**, each representing the count of pixels in the corresponding range.

---

## ⚙️ Requirements

📌 **C99 or later**
📌 **GCC compiler**
📌 **Linux/Debian OS** (for compatibility with the grading system)

---

## 🚀 Development

👨‍💻 **Oleksii Miroshnichenko**
📅 Developed as a university assignment.

---
