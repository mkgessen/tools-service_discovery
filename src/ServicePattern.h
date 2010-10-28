/**
 * \file ServicePattern.h
 * \author Christoph Mueller
 *
 * \brief Provides the base interface and several implementations for Service Patterns.
 * It encapsulate filtering routines on ServiceDescriptions in different classes.
 */

#ifndef RIMRES_SERVICEPATTERN_CORE_H_
#define RIMRES_SERVICEPATTERN_CORE_H_

#include <string>
#include <vector>
#include <boost/regex.hpp>
#include "ServiceConfiguration.h"

namespace dfki { namespace communication {

namespace pattern {

  /**
   * bitfield enumeration for all flags a service can support
   */
  enum Flags {
    BUSY = 1,
    READY = 2,
    SLAM = 4,
    MANIPULATOR = 8,
    REMOTE = 16
  };

  /**
   * simple casting function for setting flags in a service description
   */
  std::string castFlags(Flags flags);  
}

/**
 * \brief Abstract base class for ServicePattern. Provides a simple interface for
 * searching via specific service patterns (position, type, group, state, etc.)
 */
struct ServicePattern {
  /**
   * \brief General interface for filtering ServiceDescription matching a specific
   * ServicePattern.
   *
   * \param service the ServiceDescription to check whether it belongs to this pattern
   * \return true if service matches this pattern.
   */
  virtual bool matchDescription(const ServiceDescription& service) const = 0;
};

// ----------------------------------------------------------------------------

/**
 * \brief Filtering services by matching a specific property
 *
 * The PropertyPattern works on a specific label and search an expression in its
 * belonging description. Internally expression are processed with boost::regex 
 * that enables defining search patterns on descriptions via regular expressions.
 *
 * The POSIX Extended syntax is used for regular expression (used in egrep, awk)
 * http://www.boost.org/doc/libs/1_44_0/libs/regex/doc/html/boost_regex/syntax/basic_extended.html
 * 
 * \note labels will not processed with boost::regex (no regular expression usage)
 * 
 * If label = "*", PropertyPattern will search in each description.
 */
struct PropertyPattern : public ServicePattern {
  PropertyPattern(std::string label = "*", std::string expression = ".*")
   : label(label), expression(expression, boost::regex::extended) {}

  ~PropertyPattern() {}

  bool matchDescription(const ServiceDescription& service) const;

 private:
  std::string label;
  boost::regex expression;
};

// ----------------------------------------------------------------------------

/**
 * \brief Filtering services by its current position
 *
 * The Position pattern works on the location property and calculates the eucledean
 * distance for each services to a given point (x,y,z) and lists all modules that
 * are in range of a given distance.
 */
struct PositionPattern : public ServicePattern {
  PositionPattern(int x, int y, int z, double distance) 
    : x(x), y(y), z(z), distance(distance) {}
  ~PositionPattern() {}

  bool matchDescription(const ServiceDescription& service) const;

 private:
  int x, y, z;    
  double distance;
};

// ----------------------------------------------------------------------------

/**
 * \brief Filtering services by its provided flags
 * 
 * The FlagPattern works on the flags property and lists all services that matches
 * all given flags.
 */
struct FlagPattern : public ServicePattern {
  FlagPattern(int flags) : flags(flags) {}
  ~FlagPattern() {}

  bool matchDescription(const ServiceDescription& service) const;

 private:
  int flags;
};

// ----------------------------------------------------------------------------

/**
 * \brief Filtering services by a set of several service patterns.
 * 
 * For more complex search issues a multipattern can be used in order to combine
 * multiple service patterns that all need to match a service.
 *
 * Usage:
 * \verbatim
 * MultiPattern multi;
 * PropertyPattern p("type", "communication");
 * FlagPattern f(pattern::READY);
 * multi << p << f;
 * \verbatim
 *
 */
struct MultiPattern : public ServicePattern {
  MultiPattern() {}
  ~MultiPattern() {}

  bool matchDescription(const ServiceDescription& service) const;

  MultiPattern& operator<<(const ServicePattern& pattern) {
    patterns.push_back(&pattern);
    return *this;
  }

  private:
    std::vector<const ServicePattern*> patterns;
};

}}


#endif // RIMRES_SERVICEPATTERN_CORE_H_
