#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <string.h>

#include "load_shader.h"

static const char *read_file_to_string(const char *file_path) {
  FILE *file = fopen(file_path, "r");
  if (!file) {
    perror("fopen (vertex shader)");
    exit(EXIT_FAILURE);
  }

  size_t read_buffer_length = 0;
  int read_characters = 0;
  char *read_line;
  char *read_file = NULL;
  size_t read_file_size = 0;

  // read_characters includes \0
  while ((read_characters = getline(&read_line, &read_buffer_length, file)) !=
         -1) {
    // reading n characters (including \0) will make the string n characters
    // longer only if the string was at length 0 before. Otherwise it will make
    // it n - 1 characters longer
    size_t new_length = read_file_size + read_characters;
    if (read_file_size != 0)
      new_length -= 1;
    if (!(read_file = realloc(read_file, new_length))) {
      perror("realloc (read_file_to_string)");
      exit(EXIT_FAILURE);
    }
    if (read_file_size == 0)
      memset(read_file, 0, read_characters);
    // TODO: Copy string with strcat
    strcat(read_file, read_line);
    read_file_size = new_length;
    free(read_line);
    read_line = NULL;
    read_buffer_length = 0;
  }

  fclose(file);

  return read_file;
}

GLuint load_shaders(const char *vertex_file_path,
                    const char *fragment_file_path) {
  // create shaders
  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

  const char *vertex_shader = read_file_to_string(vertex_file_path);
  const char *fragment_shader = read_file_to_string(fragment_file_path);

  GLint result = GL_FALSE;
  int info_log_length;

  // compile vertex shader
  printf("Compiling shader : %s\n", vertex_file_path);
  glShaderSource(vertex_shader_id, 1, &vertex_shader, NULL);
  glCompileShader(vertex_shader_id);

  // check vertex shader
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

  if (info_log_length > 0) {
    char vertex_shader_error[info_log_length + 1];
    glGetShaderInfoLog(vertex_shader_id, info_log_length, NULL,
                       &vertex_shader_error[0]);
    fprintf(stderr, "%s\n", &vertex_shader_error[0]);
  }

  // compile fragment shader
  printf("Compiling shader : %s\n", fragment_file_path);
  glShaderSource(fragment_shader_id, 1, &fragment_shader, NULL);
  glCompileShader(fragment_shader_id);

  // check fragment shader
  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    char fragment_shader_error[info_log_length + 1];
    glGetShaderInfoLog(fragment_shader_id, info_log_length, NULL,
                       &fragment_shader_error[0]);
    fprintf(stderr, "%s\n", &fragment_shader_error[0]);
  }

  printf("Linking program\n");
  GLuint program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);

  // check program
  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);

  if (info_log_length > 0) {
    char program_error[info_log_length + 1];
    glGetProgramInfoLog(program_id, info_log_length, NULL, &program_error[0]);
    fprintf(stderr, "%s\n", &program_error[0]);
  }

  glDetachShader(program_id, vertex_shader_id);
  glDetachShader(program_id, fragment_shader_id);

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  free((void *)vertex_shader);
  free((void *)fragment_shader);

  return program_id;
}
