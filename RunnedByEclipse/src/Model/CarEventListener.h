/*
 * CarEventListener.h
 *
 *  Created on: Nov 29, 2016
 *      Author: gnouchnam
 */

#ifndef SRC_MODEL_CAREVENTLISTENER_H_
#define SRC_MODEL_CAREVENTLISTENER_H_

template <typename Event>
class CarEventListener {

public:
       virtual ~CarEventListener() {}
       virtual void OnEvent(Event& event) = 0;
};

#endif /* SRC_MODEL_CAREVENTLISTENER_H_ */
