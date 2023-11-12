//
// Created by edgar on 10/17/2023.
//

#ifndef PRACTICA08_ANIMATION_H
#define PRACTICA08_ANIMATION_H

#include <functional>
#include <vector>
namespace Animation
{

/**
 * Clase que controla una animación secuencial mediante funciones
 */
class Animation
{
  public:
	Animation();
	/**
	 * Agrega una funcion a la animacion completa.
	 * Debe retornar True si la animacion ha terminado, False si aun se ejecuta.
	 * @param function El parametro flotante es el deltaTime, la funcion debe retornar un booleano.
	 * @return Referencia a la instancia del objeto.
	 */
	Animation &addCondition(const std::function<bool(float)> &function);

	/**
	 * Prepara la animacion estableciendo la primera funcion agregada como la que empezara.
	 */
	void prepare();

	/**
	 * Actualiza el estado de la animacion.
	 * @param deltaTime dt, enviado desde el bucle principal.
	 */
	void update(float deltaTime);

	/**
	 * Starts the animation.
	 */
	void start() { this->started = true; }

	/**
	 * @return Return the animation status. 
	 */
	[[nodiscard]] bool isStarted() const { return started; }

  private:
	int currentIndex;
	bool started = false;
	std::function<bool(float)> *current;
	std::vector<std::function<bool(float)>> conditions;
};

} // namespace Animation
#endif // PRACTICA08_ANIMATION_H
