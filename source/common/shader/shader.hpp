#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            //TODO: (Req 1) Create A shader program
            // glCreateProgram creates an empty program object and returns a non-zero value by which it can be referenced.
            program = glCreateProgram();
        }
        ~ShaderProgram(){
            //TODO: (Req 1) Delete a shader program
            // glDeleteProgram frees the memory and invalidates the name associated with the program object specified by program. This command effectively undoes the effects of a call to glCreateProgram.
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //TODO: (Req 1) Return the location of the uniform with the given name
            // glGetUniformLocation returns an integer that represents the location of a specific uniform variable within a program object.
            return glGetUniformLocation(program, name.c_str());
        }

        // The commands glUniform{1|2|3|4}{f|i|ui} are used to change the value of the uniform variable specified by location using the values passed as arguments. The number specified in the command should match the number of components in the data type of the specified uniform variable (e.g., 1 for float, int, unsigned int, bool; 2 for vec2, ivec2, uvec2, bvec2, etc.). The suffix f indicates that floating-point values are being passed; the suffix i indicates that integer values are being passed; the suffix ui indicates that unsigned integer values are being passed, and this type should also match the data type of the specified uniform variable.
        void set(const std::string &uniform, GLfloat value) {
            //TODO: (Req 1) Send the given float value to the given uniform
            glUniform1f(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLuint value) {
            //TODO: (Req 1) Send the given unsigned integer value to the given uniform
            glUniform1ui(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLint value) {
            //TODO: (Req 1) Send the given integer value to the given uniform
            glUniform1i(getUniformLocation(uniform), value);
        }

        // The commands glUniform{1|2|3|4}{f|i|ui}v can be used to modify a single uniform variable or a uniform variable array. These commands pass a count and a pointer to the values to be loaded into a uniform variable or a uniform variable array. A count of 1 should be used if modifying the value of a single uniform variable, and a count of 1 or greater can be used to modify an entire array or part of an array. 
        void set(const std::string &uniform, glm::vec2 value) {
            //TODO: (Req 1) Send the given 2D vector value to the given uniform
            glUniform2fv(getUniformLocation(uniform), 1, glm::value_ptr(value));
        }

        void set(const std::string &uniform, glm::vec3 value) {
            //TODO: (Req 1) Send the given 3D vector value to the given uniform
            glUniform3fv(getUniformLocation(uniform), 1, glm::value_ptr(value));
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //TODO: (Req 1) Send the given 4D vector value to the given uniform
            glUniform4fv(getUniformLocation(uniform), 1, glm::value_ptr(value));
        }

        // void glUniformMatrix4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
        void set(const std::string &uniform, glm::mat4 matrix) {
            //TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(matrix));
        }

        //TODO: (Req 1) Delete the copy constructor and assignment operator.
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        //Question: Why do we delete the copy constructor and assignment operator?
        //Answer: We delete the copy constructor and assignment operator because we don't want to have multiple instances of the same shader program. I think that we deleted the copy constructor and assignment operator to prevent accidentally overriding values of the shader program read from the config file during development in other parts of the code. You can create multiple instances of the class each holding a fragment shader and vertex shader but only one of them would be active during rendering.
    };

}

#endif